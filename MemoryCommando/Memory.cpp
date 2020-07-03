#include "Memory.h"


#include <stdexcept>
#include <wil/resource.h>



#include "CreateToolhelp32SnapshotException.h"
#include "Process32Exception.h"
#include <boost/algorithm/string.hpp>

#include "Module32Exception.h"

namespace MemoryCommando::Memory {
    using namespace Exceptions;

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

    PROCESSENTRY32W GetProcess(const DWORD processId) {
        std::vector<PROCESSENTRY32W> processes = Memory::GetRunningProcesses();

        for(auto currentProcess : processes) {
            if(processId == currentProcess.th32ProcessID)
                return currentProcess;
        }

        throw std::runtime_error("Couldn't find a process with the specified processId in the process list.");
    }

    PROCESSENTRY32W GetProcess(HANDLE processHandle) {
        const DWORD processId = GetProcessId(processHandle);
        const PROCESSENTRY32W process = GetProcess(processId);

        return process;
    }

    PROCESSENTRY32W GetProcess(const std::wstring& processName, const size_t processNumber) {
        auto processes = Memory::GetRunningProcesses();

        size_t foundProcessNumber = 0;
        for(auto currentProcess : processes) {
            if(boost::iequals(processName, currentProcess.szExeFile)) {
                foundProcessNumber++;

                if(foundProcessNumber >= processNumber) {
                    return currentProcess;
                }
            }
        }

        throw std::runtime_error("Couldn't find a process with the specified process name in the process list.");
    }

    std::vector<MODULEENTRY32W> GetModules(const DWORD processId) {
        std::vector<MODULEENTRY32W> modules{};
        MODULEENTRY32               module{};
        module.dwSize = sizeof(module);

        const auto modulesSnapshot = wil::unique_tool_help_snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId));

        if(!modulesSnapshot)
            throw CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot of modules.", GetLastError());

        bool copiedToBuffer = Module32First(modulesSnapshot.get(), &module);
        if(!copiedToBuffer)
            throw Module32Exception("Module32First failed to fill the buffer.", GetLastError());

        do {
            modules.push_back(module);
            copiedToBuffer = Module32Next(modulesSnapshot.get(), &module);
        } while(copiedToBuffer);

        if(GetLastError() != ERROR_NO_MORE_FILES)
            throw Module32Exception("Module32Next failed to fill the buffer.", GetLastError());

        return modules;
    }


    MODULEENTRY32W GetModule(const std::wstring& moduleName, const DWORD processId) {
        std::vector<MODULEENTRY32W> modules = GetModules(processId);

        for(auto currentModule : modules) {
            if(boost::algorithm::iequals(moduleName, currentModule.szModule))
                return currentModule;
        }

        throw std::runtime_error("Couldn't find a module with the specified name in the modules list.");
    }

    uintptr_t GetModuleBaseAddress(const std::wstring& moduleName, DWORD processId) {
        auto module = GetModule(moduleName, processId);

        return uintptr_t(module.modBaseAddr);
    }

    size_t GetModuleSize(const std::wstring& moduleName, DWORD processId) {
        const auto module = GetModule(moduleName, processId);
        return size_t(module.modBaseSize);
    }
}
