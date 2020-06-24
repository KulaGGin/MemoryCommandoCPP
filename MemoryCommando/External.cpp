#include "External.h"

#include <Psapi.h>
#include <boost/locale/encoding_utf.hpp>
#include "CreateToolhelp32SnapshotException.h"
#include "GetProcessImageFileNameException.h"
#include "Module32Exception.h"
#include "Process32Exception.h"

namespace MemoryCommando::External {
    using namespace Exceptions;
    namespace conv = boost::locale::conv;
    namespace algorithm = boost::algorithm;
    namespace locale = boost::locale;

    PROCESSENTRY32W GetProcess(DWORD processId) {
        std::vector<PROCESSENTRY32W> processes = GetRunningProcesses();
        PROCESSENTRY32W wantedProcess{};

        for (auto currentProcess : processes) {
            if (processId == currentProcess.th32ProcessID)
                wantedProcess = currentProcess;
        }

        return wantedProcess;
    }

    PROCESSENTRY32W GetProcess(const std::string& processName, const size_t processNumber) {
        const std::wstring processNameWide = boost::locale::conv::utf_to_utf<WCHAR>(processName);
        const PROCESSENTRY32W wantedProcess = GetProcess(processNameWide, processNumber);
        return wantedProcess;
    }

    PROCESSENTRY32W GetProcess(const std::wstring& processName, size_t processNumber) {
        auto processes = GetRunningProcesses();
        PROCESSENTRY32W wantedProcess{};

        size_t currentProcessNumber = 0;
        for (auto currentProcess : processes) {
            if (boost::iequals(processName, currentProcess.szExeFile)) {
                currentProcessNumber++;

                if (currentProcessNumber >= processNumber) {
                    wantedProcess = currentProcess;
                    break;
                }
            }
        }

        return wantedProcess;
    }

    HANDLE GetProcessHandle(const DWORD processId, const DWORD processAccess) {
        const HANDLE processHandle = ::OpenProcess(processAccess, 0, processId);

        if (!processHandle)
            throw Process32Exception("OpenProcess couldn't get a handle to the specified processId", GetLastError());

        return processHandle;
    }

    HANDLE GetProcessHandle(const std::wstring& processName, const size_t processNumber, const DWORD processAccess) {
        const PROCESSENTRY32W process = GetProcess(processName, processNumber);
        const DWORD  processId = process.th32ProcessID;
        const HANDLE processHandle = GetProcessHandle(processId, processAccess);

        return processHandle;
    }

    std::wstring GetProcessName(HANDLE processHandle) {
        const auto processNameBuffer = std::unique_ptr<WCHAR[]>(new WCHAR[MAX_PATH]);

        const DWORD nameLength = GetProcessImageFileName(processHandle, processNameBuffer.get(), sizeof(WCHAR) * MAX_PATH);

        if (!nameLength)
            throw GetProcessImageFileNameException("Couldn't get process name from the processHandle.", GetLastError());

        const std::wstring processName(processNameBuffer.get());

        return processName;
    }

    std::wstring GetProcessName(DWORD processId) {
        const HANDLE processHandle = GetProcessHandle(processId, PROCESS_QUERY_LIMITED_INFORMATION);
        const std::wstring processName = GetProcessName(processHandle);

        return processName;
    }

    std::vector<PROCESSENTRY32W> GetRunningProcesses() {
        std::vector<PROCESSENTRY32W> runningProcesses;
        PROCESSENTRY32 process;
        ZeroMemory(&process, sizeof(process));

        const wil::unique_tool_help_snapshot processesSnapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
        if (!processesSnapshot)
            throw CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot.", GetLastError());

        bool copiedToBuffer = Process32First(processesSnapshot.get(), &process);
        if (!copiedToBuffer)
            throw Process32Exception("Process32First failed to fill the buffer.", GetLastError());

        while(copiedToBuffer) {
            runningProcesses.push_back(process);
            copiedToBuffer = Process32Next(processesSnapshot.get(), &process);
        }

        if(!copiedToBuffer && GetLastError() != ERROR_NO_MORE_FILES)
            throw Process32Exception("Process32Next failed to fill the buffer.", GetLastError());

        return runningProcesses;
    }

    DWORD GetProcessId(const std::wstring& processName, const size_t processNumber) {
        DWORD          processId = NULL;
        PROCESSENTRY32 process;
        ZeroMemory(&process, sizeof(process));
        process.dwSize = sizeof(process);

        // todo create GetProcessList method and extract code into it
        const wil::unique_tool_help_snapshot processesSnapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
        if (!processesSnapshot)
            throw CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot.",
                GetLastError());

        size_t currentProcessNumber = 0;
        bool   copiedToBuffer = Process32First(processesSnapshot.get(), &process);
        if (!copiedToBuffer)
            throw Process32Exception("Process32First failed to fill the buffer.", GetLastError());

        while (copiedToBuffer) {
            std::wstring currentProcessNameWide(conv::utf_to_utf<WCHAR>(process.szExeFile));
            if (boost::iequals(processName, currentProcessNameWide)) {
                currentProcessNumber++;
                if (currentProcessNumber >= processNumber) {
                    processId = process.th32ProcessID;
                    break;
                }
            }

            copiedToBuffer = Process32Next(processesSnapshot.get(), &process);
        }

        if (!copiedToBuffer) {
            if (GetLastError() != ERROR_NO_MORE_FILES)
                throw Process32Exception("Process32Next failed to fill the buffer.", GetLastError());
            if (GetLastError() == ERROR_NO_MORE_FILES) {
                throw
                    Process32Exception("Process32Next iterated through the last process in the list and couldn't find the name specified.",
                        GetLastError());
            }
        }

        return processId;
    }

    DWORD GetProcessId(HANDLE processHandle) {
        return ::GetProcessId(processHandle);
    }

    std::vector<HMODULE> GetModulesHandles(HANDLE processHandle) {
        std::vector<HMODULE>       modulesHandlesVector;
        std::shared_ptr<HMODULE[]> modulesHandlesSmartArray;
        size_t                     modulesHandlesArrayLength{ 128 };
        DWORD                      bytesNeeded;

        bool queryResult;
        while (true) {
            modulesHandlesSmartArray = std::shared_ptr<HMODULE[]>(new HMODULE[modulesHandlesArrayLength]);
            const size_t modulesArraySizeBytes = modulesHandlesArrayLength * sizeof(HMODULE);
            queryResult = EnumProcessModules(processHandle, modulesHandlesSmartArray.get(), modulesArraySizeBytes,
                &bytesNeeded);

            if (modulesArraySizeBytes < bytesNeeded) {
                modulesHandlesArrayLength = bytesNeeded / sizeof(HMODULE) + 1;
                continue;
            }

            if (queryResult)
                break;
        }


        const size_t modulesHandlesNumber = bytesNeeded / sizeof(HMODULE);

        for (size_t index = 0; index < modulesHandlesNumber; ++index) {
            modulesHandlesVector.push_back(modulesHandlesSmartArray[index]);
        }

        return modulesHandlesVector;
    }

    MODULEENTRY32W GetModule(const DWORD processId, const std::wstring& moduleName) {
        std::vector<MODULEENTRY32W> modules = GetModules(processId);
        MODULEENTRY32W wantedModule{};

        for (auto currentModule : modules) {
            if (algorithm::iequals(moduleName, currentModule.szModule)) {
                wantedModule = currentModule;
            }
        }

        return wantedModule;
    }

    std::vector<MODULEENTRY32W> GetModules(const DWORD processId) {
        std::vector<MODULEENTRY32W> modulesSequence;
        MODULEENTRY32               module;
        ZeroMemory(&module, sizeof(module));
        module.dwSize = sizeof(module);

        const auto modulesSnapshot = wil::
            unique_tool_help_snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId));
        if (!modulesSnapshot)
            throw CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot.",
                GetLastError());

        bool copiedToBuffer = Module32First(modulesSnapshot.get(), &module);
        if (!copiedToBuffer)
            throw Module32Exception("Module32First failed to fill the buffer.", GetLastError());
        while (copiedToBuffer) {
            modulesSequence.push_back(module);
            copiedToBuffer = Module32Next(modulesSnapshot.get(), &module);
        }

        if (GetLastError() != ERROR_NO_MORE_FILES)
            throw Module32Exception("Module32Next failed to fill the buffer.", GetLastError());

        return modulesSequence;
    }

    HANDLE OpenProcess(DWORD processId, DWORD processAccess) {
        const HANDLE processHandle = ::OpenProcess(processAccess, 0, processId);
        if (!processHandle) {
            throw Process32Exception("OpenProcess couldn't get a handle to the specified processId", GetLastError());
        }

        return processHandle;
    }

    HANDLE OpenProcess(const std::wstring& processName, size_t processNumber, DWORD processAccess) {
        const DWORD  processId = GetProcessId(processName, processNumber);
        const HANDLE processHandle = OpenProcess(processId, processAccess);
        return processHandle;
    }
}