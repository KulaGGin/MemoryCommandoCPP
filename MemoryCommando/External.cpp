#include "External.h"

#include <Psapi.h>
#include <boost/locale/encoding_utf.hpp>
#include "CreateToolhelp32SnapshotException.h"
#include "GetProcessIdException.h"
#include "GetProcessImageFileNameException.h"
#include "Module32Exception.h"
#include "Process32Exception.h"
#include "GetProcessIdException.h"

namespace MemoryCommando::External {
    using namespace Exceptions;
    namespace conv = boost::locale::conv;
    namespace algorithm = boost::algorithm;
    namespace locale = boost::locale;

    PROCESSENTRY32W GetProcess(DWORD processId) {
        std::vector<PROCESSENTRY32W> processes = GetRunningProcesses();

        for (auto currentProcess : processes) {
            if (processId == currentProcess.th32ProcessID)
                return currentProcess;
        }

        throw std::runtime_error("Couldn't find a process with the specified processId in the process list.");
    }

    PROCESSENTRY32W GetProcess(const std::string& processName, const size_t processNumber) {
        const std::wstring processNameWide = boost::locale::conv::utf_to_utf<WCHAR>(processName);
        const PROCESSENTRY32W wantedProcess = GetProcess(processNameWide, processNumber);
        return wantedProcess;
    }

    PROCESSENTRY32W GetProcess(const std::wstring& processName, size_t processNumber) {
        auto processes = GetRunningProcesses();

        size_t foundProcessNumber = 0;
        for (auto currentProcess : processes) {
            if (boost::iequals(processName, currentProcess.szExeFile)) {
                foundProcessNumber++;

                if (foundProcessNumber >= processNumber) {
                    return currentProcess;
                }
            }
        }

        throw std::runtime_error("Couldn't find a process with the specified process name in the process list.");
    }

    DWORD GetProcessId(const HANDLE processHandle) {
        const DWORD processId = ::GetProcessId(processHandle);

        if(!processId)
            throw GetProcessIdException("Couldn't get processId from the specified processHandle", GetLastError());

        return processId;
    }

    DWORD GetProcessId(const std::wstring& processName, const size_t processNumber) {
        const auto process = GetProcess(processName, processNumber);
        const DWORD processId = process.th32ProcessID;

        return processId;
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
        const auto processName = GetProcessName(GetProcessId(processHandle));

        return processName;
    }

    std::wstring GetProcessName(DWORD processId) {
        const auto process = GetProcess(processId);
        const std::wstring processName = process.szExeFile;

        return processName;
    }

    std::vector<PROCESSENTRY32W> GetRunningProcesses() {
        std::vector<PROCESSENTRY32W> runningProcesses;
        PROCESSENTRY32 process;
        ZeroMemory(&process, sizeof(process));
        process.dwSize = sizeof(process);

        const wil::unique_tool_help_snapshot processesSnapshot{CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)};

        if(!processesSnapshot)
            throw CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot.", GetLastError());

        bool copiedToBuffer = Process32First(processesSnapshot.get(), &process);
        if(!copiedToBuffer)
            throw Process32Exception("Process32First failed to fill the buffer.", GetLastError());

        do {
            runningProcesses.push_back(process);
            copiedToBuffer = Process32Next(processesSnapshot.get(), &process);
        } while (copiedToBuffer);

        if(!copiedToBuffer && GetLastError() != ERROR_NO_MORE_FILES)
            throw Process32Exception("Process32Next failed to fill the buffer.", GetLastError());

        return runningProcesses;
    }

    std::vector<MODULEENTRY32W> GetModules(const DWORD processId) {
        std::vector<MODULEENTRY32W> modules;
        MODULEENTRY32               module;
        ZeroMemory(&module, sizeof(module));
        module.dwSize = sizeof(module);

        const auto modulesSnapshot = wil::unique_tool_help_snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId));

        if (!modulesSnapshot)
            throw CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot.", GetLastError());

        bool copiedToBuffer = Module32First(modulesSnapshot.get(), &module);
        if (!copiedToBuffer)
            throw Module32Exception("Module32First failed to fill the buffer.", GetLastError());

        do {
            modules.push_back(module);
            copiedToBuffer = Module32Next(modulesSnapshot.get(), &module);
        } while (copiedToBuffer);

        if (GetLastError() != ERROR_NO_MORE_FILES)
            throw Module32Exception("Module32Next failed to fill the buffer.", GetLastError());

        return modules;
    }

    MODULEENTRY32W GetModule(const DWORD processId, const std::wstring& moduleName) {
        std::vector<MODULEENTRY32W> modules = GetModules(processId);

        for (auto currentModule : modules) {
            if (algorithm::iequals(moduleName, currentModule.szModule))
                return currentModule;
        }

        throw std::runtime_error("Couldn't find a module with the specified name in the modules list.");
    }

    uintptr_t GetModuleBaseAddress(DWORD processId, const std::wstring& moduleName) {
        auto module = GetModule(processId, moduleName);
        return uintptr_t(module.modBaseAddr);
    }
