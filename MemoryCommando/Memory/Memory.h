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

    DWORD GetProcessId(HANDLE processHandle);
    DWORD GetProcessId(const std::wstring& processName, size_t processNumber = 1);

    HANDLE GetProcessHandle(DWORD processId, DWORD processAccess = PROCESS_ALL_ACCESS);
    HANDLE GetProcessHandle(const std::wstring& processName, size_t processNumber = 1, DWORD processAccess = PROCESS_ALL_ACCESS);

    std::wstring GetProcessName(HANDLE processHandle);
    std::wstring GetProcessName(DWORD processId);

    std::vector<MODULEENTRY32W> GetModules(DWORD processId);
    std::vector<MODULEENTRY32W> GetModules(HANDLE processHandle);
    std::vector<MODULEENTRY32W> GetModules(const std::wstring& processName, size_t processNumber = 1);

    MODULEENTRY32W GetModule(const std::wstring& moduleName, DWORD processId);
    MODULEENTRY32W GetModule(const std::wstring& moduleName, HANDLE processHandle);
    MODULEENTRY32W GetModule(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber = 1);

    uintptr_t GetModuleBaseAddress(const std::wstring& moduleName, DWORD processId);
    uintptr_t GetModuleBaseAddress(const std::wstring& moduleName, HANDLE processHandle);
    uintptr_t GetModuleBaseAddress(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber = 1);

    size_t GetModuleSize(const std::wstring& moduleName, DWORD processId);
    size_t GetModuleSize(const std::wstring& moduleName, HANDLE processHandle);
    size_t GetModuleSize(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber = 1);
}

