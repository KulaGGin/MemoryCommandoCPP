#include "MemoryManagerExternal.h"

#include <locale>
#include <Psapi.h>

#include "HelperMethods.h"
#include "ProcessAccess.h"
#include "CreateToolhelp32SnapshotException.h"
#include "Process32Exception.h"
#include <wil/resource.h>
#include <boost/locale.hpp>
#include <boost/locale/conversion.hpp>
#include <boost/algorithm/string.hpp>

namespace MemoryCommanderCpp {

    using namespace Exceptions;
    using Shared::HelperMethods;
    namespace conv = boost::locale::conv;
    namespace algorithm = boost::algorithm;
    namespace locale = boost::locale;

    MemoryManagerExternal::MemoryManagerExternal(const DWORD processId, const DWORD processAccess) {
        _processHandle = OpenProcess(processId, processAccess);
    }

    MemoryManagerExternal::MemoryManagerExternal(const std::wstring& processName, size_t processNumber, DWORD processAccess) {
        _processHandle = OpenProcess(processName, processNumber, processAccess);
    }

    MemoryManagerExternal::MemoryManagerExternal(const std::string& processName, size_t processNumber, DWORD processAccess) {
        const std::wstring processNameWide = conv::utf_to_utf<WCHAR>(processName);
        _processHandle = OpenProcess(processNameWide, processNumber, processAccess);
    }

    std::vector<HMODULE> MemoryManagerExternal::GetModulesHandles() const {
        std::vector<HMODULE> modulesHandlesVector;
        std::shared_ptr<HMODULE[]> modulesHandlesSmartArray;
        size_t modulesHandlesArrayLength{128};
        DWORD bytesNeeded;

        bool queryResult = false;
        while(true) {
            modulesHandlesSmartArray = std::shared_ptr<HMODULE[]>(new HMODULE[modulesHandlesArrayLength]);
            const size_t modulesArraySizeBytes = modulesHandlesArrayLength * sizeof(HMODULE);
            queryResult = EnumProcessModules(_processHandle, modulesHandlesSmartArray.get(), modulesArraySizeBytes, &bytesNeeded);

            if(modulesArraySizeBytes < bytesNeeded) {
                modulesHandlesArrayLength = bytesNeeded / sizeof(HMODULE) + 1;
                continue;
            }

            if(queryResult)
                break;
        }

        
        const size_t modulesHandlesNumber = bytesNeeded / sizeof(HMODULE);

        for(size_t index = 0; index < modulesHandlesNumber; ++index) {
            modulesHandlesVector.push_back(modulesHandlesSmartArray[index]);
        }

        return modulesHandlesVector;
    }

    //vector<MODULEENTRY32W> MemoryManagerExternal::GetModules() const {
    //}

    //HMODULE MemoryManagerExternal::GetModule(wstring moduleName) {
    //    HMODULE module;
    //    vector<HMODULE> modules = GetModulesHandles();
    //    for(auto module : modules) {
    //        module.name
    //    }

    //    return module;
    //}

    HANDLE MemoryManagerExternal::OpenProcess(DWORD processId, DWORD processAccess) {
        const HANDLE processHandle = ::OpenProcess(processAccess, 0, processId);
        if(!processHandle) {
            throw Process32Exception("OpenProcess couldn't get a handle to the specified processId", GetLastError());
        }

        return processHandle;
    }

    HANDLE MemoryManagerExternal::OpenProcess(const std::wstring& processName, size_t processNumber, DWORD processAccess) {
        const DWORD processId = GetProcessId(processName, processNumber);
        const HANDLE processHandle = OpenProcess(processId, processAccess);
        return processHandle;
    }

    DWORD MemoryManagerExternal::GetProcessId(const std::wstring& processName, size_t processNumber) const {
        DWORD processId = NULL;
        PROCESSENTRY32 process;
        ZeroMemory(&process, sizeof(process));
        process.dwSize = sizeof(process);

        // todo create GetProcessList method and extract code into it
        const wil::unique_tool_help_snapshot toolHelp32Snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
        if (!toolHelp32Snapshot)
            throw CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot.", GetLastError());

        size_t currentProcessNumber = 0;
        bool copiedToBuffer = Process32First(toolHelp32Snapshot.get(), &process);
        if (!copiedToBuffer)
            throw Process32Exception("Process32First failed to fill the buffer.", GetLastError());

        while (copiedToBuffer) {
            std::wstring currentProcessNameWide(conv::utf_to_utf<WCHAR>(process.szExeFile));
            if(boost::iequals(processName, currentProcessNameWide)) {
                currentProcessNumber++;
                if(currentProcessNumber >= processNumber) {
                    processId = process.th32ProcessID;
                    break;
                }
            }

            copiedToBuffer = Process32Next(toolHelp32Snapshot.get(), &process);
        }

        if(!copiedToBuffer) {
            if(GetLastError() != ERROR_NO_MORE_FILES)
                throw Process32Exception("Process32Next failed to fill the buffer.", GetLastError());
            if(GetLastError() == ERROR_NO_MORE_FILES) {
                throw Process32Exception("Process32Next iterated through the last process in the list and couldn't find the name specified.", GetLastError());
            }
        }

        return processId;
    }
}
