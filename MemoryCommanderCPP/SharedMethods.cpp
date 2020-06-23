#include "SharedMethods.h"



namespace MemoryCommanderCpp {

    DWORD GetProcessId(const std::wstring& processName, const size_t processNumber) {
        DWORD          processId = NULL;
        PROCESSENTRY32 process;
        ZeroMemory(&process, sizeof(process));
        process.dwSize = sizeof(process);

        // todo create GetProcessList method and extract code into it
        const wil::unique_tool_help_snapshot processesSnapshot{CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)};
        if(!processesSnapshot)
            throw CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot.",
                                                    GetLastError());

        size_t currentProcessNumber = 0;
        bool   copiedToBuffer = Process32First(processesSnapshot.get(), &process);
        if(!copiedToBuffer)
            throw Process32Exception("Process32First failed to fill the buffer.", GetLastError());

        while(copiedToBuffer) {
            std::wstring currentProcessNameWide(conv::utf_to_utf<WCHAR>(process.szExeFile));
            if(boost::iequals(processName, currentProcessNameWide)) {
                currentProcessNumber++;
                if(currentProcessNumber >= processNumber) {
                    processId = process.th32ProcessID;
                    break;
                }
            }

            copiedToBuffer = Process32Next(processesSnapshot.get(), &process);
        }

        if(!copiedToBuffer) {
            if(GetLastError() != ERROR_NO_MORE_FILES)
                throw Process32Exception("Process32Next failed to fill the buffer.", GetLastError());
            if(GetLastError() == ERROR_NO_MORE_FILES) {
                throw
                        Process32Exception("Process32Next iterated through the last process in the list and couldn't find the name specified.",
                                           GetLastError());
            }
        }

        return processId;
    }

    std::vector<MODULEENTRY32W> GetModules(const DWORD& processId) {
        std::vector<MODULEENTRY32W> modulesSequence{};
        MODULEENTRY32               module;
        ZeroMemory(&module, sizeof(module));
        module.dwSize = sizeof(module);

        const auto modulesSnapshot = wil::
                unique_tool_help_snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId));
        if(!modulesSnapshot)
            throw CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot.",
                                                    GetLastError());

        bool copiedToBuffer = Module32First(modulesSnapshot.get(), &module);
        if(!copiedToBuffer)
            throw Module32Exception("Module32First failed to fill the buffer.", GetLastError());
        while(copiedToBuffer) {
            modulesSequence.push_back(module);
            copiedToBuffer = Module32Next(modulesSnapshot.get(), &module);
        }

        if(GetLastError() != ERROR_NO_MORE_FILES)
            throw Module32Exception("Module32Next failed to fill the buffer.", GetLastError());

        return modulesSequence;
    }
}
