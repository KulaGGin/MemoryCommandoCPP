#pragma once
#include <windows.h>

#include <boost/locale/encoding_utf.hpp>
#include <string>
#include <TlHelp32.h>
#include <wil/resource.h>
#include <boost/algorithm/string.hpp>
#include <Psapi.h>
#include "CreateToolhelp32SnapshotException.h"
#include "Process32Exception.h"
#include <string>
#include "Module32Exception.h"

namespace MemoryCommando::External {
    std::vector<PROCESSENTRY32> GetRunningProcesses();

    DWORD GetProcessId(const std::wstring& processName, const size_t processNumber);
    DWORD GetProcessId(HANDLE processHandle);

    HANDLE OpenProcess(DWORD processId, DWORD processAccess);
    HANDLE OpenProcess(const std::wstring& processName, size_t processNumber, DWORD processAccess);

    std::vector<MODULEENTRY32W> GetModules(DWORD processId);
    std::vector<HMODULE> GetModulesHandles(HANDLE processHandle);
    MODULEENTRY32W GetModule(DWORD processId, const std::wstring& moduleName);
}