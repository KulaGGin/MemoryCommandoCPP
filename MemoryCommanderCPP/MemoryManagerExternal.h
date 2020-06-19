#pragma once
#include <windows.h>
#include <string>
#include <TlHelp32.h>
#include <vector>


namespace MemoryCommanderCpp {
    class MemoryManagerExternal {
    public:
        MemoryManagerExternal(int processId);
        MemoryManagerExternal(const std::string& processName);
        std::vector<MODULEENTRY32> GetModules();
        std::vector<MODULEENTRY32> GetModule(std::string moduleName);
        DWORD_PTR GetModuleBaseAddress(std::string moduleName);
        size_t GetModuleSize(std::string moduleName);
        SYSTEM_INFO GetProcessInfo();
        DWORD_PTR AllocateVirtualMemory(DWORD_PTR baseAddress, size_t allocationSize, )
    private:
        void* processHandle = nullptr;

        void* OpenProcess(int processId);
        void* OpenProcess(const std::string& processName);

    };
}
