#pragma once
#include <windows.h>

#include <string>
#include <TlHelp32.h>
#include <wil/resource.h>
#include <boost/algorithm/string.hpp>
#include <string>

namespace MemoryCommando::External {
    std::vector<PROCESSENTRY32W> GetRunningProcesses();

    PROCESSENTRY32W GetProcess(DWORD processId);
    PROCESSENTRY32W GetProcess(const std::string& processName, size_t processNumber = 1);
    PROCESSENTRY32W GetProcess(const std::wstring& processName, size_t processNumber = 1);

    DWORD GetProcessId(HANDLE processHandle);
    DWORD GetProcessId(const std::wstring& processName, size_t processNumber = 1);

    HANDLE GetProcessHandle(DWORD processId, DWORD processAccess = PROCESS_ALL_ACCESS);
    HANDLE GetProcessHandle(const std::wstring& processName, size_t processNumber = 1, DWORD processAccess = PROCESS_ALL_ACCESS);

    std::wstring GetProcessName(HANDLE processHandle);
    std::wstring GetProcessName(DWORD processId);

    std::vector<MODULEENTRY32W> GetModules(DWORD processId);
    MODULEENTRY32W GetModule(DWORD processId, const std::wstring& moduleName);
    uintptr_t GetModuleBaseAddress(DWORD processId, const std::wstring& moduleName);
}