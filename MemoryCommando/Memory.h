#pragma once
#include <string>
#include <vector>
#include <windows.h>

#include <tlhelp32.h>


namespace MemoryCommando::Memory {
    SYSTEM_INFO GetSystemInfo();
    std::vector<PROCESSENTRY32W> GetRunningProcesses();
    PROCESSENTRY32W GetProcess(DWORD processId);
    PROCESSENTRY32W GetProcess(HANDLE processHandle);
    PROCESSENTRY32W GetProcess(const std::wstring& processName, const size_t processNumber);

    std::vector<MODULEENTRY32W> GetModules(DWORD processId);
    MODULEENTRY32W GetModule(const std::wstring& moduleName, DWORD processId);
    uintptr_t GetModuleBaseAddress(const std::wstring& moduleName, DWORD processId);
    size_t GetModuleSize(const std::wstring& moduleName, DWORD processId);
}

