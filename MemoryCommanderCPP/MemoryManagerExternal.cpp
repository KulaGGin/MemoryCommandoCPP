#include "MemoryManagerExternal.h"

#include <locale>



#include "HelperMethods.h"
#include "ProcessAccess.h"

namespace MemoryCommanderCpp {
    using MemoryCommanderCPP::Shared::HelperMethods;

    MemoryManagerExternal::MemoryManagerExternal(DWORD processId, DWORD processAccess) {
        processHandle = OpenProcess(processId, processAccess);
    }

    MemoryManagerExternal::MemoryManagerExternal(const std::wstring& processName, size_t processNumber, DWORD processAccess) {
        processHandle = OpenProcess(processName, processNumber, processAccess);
    }

    MemoryManagerExternal::MemoryManagerExternal(const std::string& processName, size_t processNumber, DWORD processAccess) {
        const std::wstring processNameWide = HelperMethods::ConvertStringToWString(processName);
        processHandle = OpenProcess(processNameWide, processNumber, processAccess);
    }

    HANDLE MemoryManagerExternal::OpenProcess(DWORD processId, DWORD processAccess) {
        const HANDLE processHandle = ::OpenProcess(processAccess, 0, processId);
        return processHandle;
    }

    HANDLE MemoryManagerExternal::OpenProcess(const std::wstring& processName, size_t processNumber, DWORD processAccess) {
        const DWORD processId = GetProcessIdByName(processName, processNumber);
        const HANDLE processHandle = OpenProcess(processId, processAccess);
        return processHandle;
    }

    DWORD MemoryManagerExternal::GetProcessIdByName(const std::wstring& processName, size_t processNumber = 1) const {
        DWORD processId = NULL;
        PROCESSENTRY32 process;
        ZeroMemory(&process, sizeof(process));
        process.dwSize = sizeof(process);

        const HANDLE toolHelp32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        size_t currentProcessNumber = 0;
        bool copiedToBuffer = Process32First(toolHelp32Snapshot, &process);
        while (copiedToBuffer) {
            if(!_wcsicmp(&processName[0], process.szExeFile)) {
                currentProcessNumber++;
                if(currentProcessNumber >= processNumber) {
                    processId = process.th32ProcessID;
                    break;
                }
            }

            copiedToBuffer = Process32Next(toolHelp32Snapshot, &process);
        }

        CloseHandle(toolHelp32Snapshot);
        return processId;
    }
}