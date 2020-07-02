#include "Memory.h"

#include <wil/resource.h>



#include "CreateToolhelp32SnapshotException.h"
#include "Process32Exception.h"

namespace MemoryCommando::Memory {
    SYSTEM_INFO GetSystemInfo() {
        SYSTEM_INFO systemInfo;
        ::GetSystemInfo(&systemInfo);
        return systemInfo;
    }

    std::vector<PROCESSENTRY32W> GetRunningProcesses() {
        std::vector<PROCESSENTRY32W> runningProcesses{};
        PROCESSENTRY32 process{};
        process.dwSize = sizeof(process);

        const wil::unique_tool_help_snapshot processesSnapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

        if(!processesSnapshot)
            throw Exceptions::CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot.", GetLastError());

        bool copiedToBuffer = Process32First(processesSnapshot.get(), &process);
        if(!copiedToBuffer)
            throw Exceptions::Process32Exception("Process32First failed to fill the buffer.", GetLastError());

        do {
            runningProcesses.push_back(process);
            copiedToBuffer = Process32Next(processesSnapshot.get(), &process);
        } while(copiedToBuffer);

        if(!copiedToBuffer && GetLastError() != ERROR_NO_MORE_FILES)
            throw Exceptions::Process32Exception("Process32Next failed to fill the buffer.", GetLastError());

        return runningProcesses;
    }
}
