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
    size_t GetModuleSize(DWORD processId, const std::wstring& moduleName);

    uintptr_t AllocateVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, size_t allocationSize, DWORD allocationType = MEM_RESERVE | MEM_COMMIT, DWORD protectionType = PAGE_EXECUTE_READWRITE);
    void FreeVirtualMemory(HANDLE processHandle, uintptr_t address, DWORD freeType = MEM_RELEASE, size_t size = 0);
    void ProtectVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, size_t protectionSize, DWORD protectionType);
    
    std::vector<BYTE> ReadVirtualMemory(HANDLE processHandle, uintptr_t address, int bytesNumber);
    std::vector<BYTE> ReadVirtualMemory(HANDLE processHandle, std::vector<uintptr_t> pointers, int bytesNumber);
    std::vector<BYTE> ReadVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, std::vector<uintptr_t> offsets, int bytesNumber);
    std::vector<BYTE> ReadVirtualMemory(HANDLE processHandle, const std::wstring& moduleName, uintptr_t offset, int bytesNumber);
    std::vector<BYTE> ReadVirtualMemory(HANDLE processHandle, std::wstring moduleName, std::vector<uintptr_t> offsets, int bytesNumber);
    // todo ReadVirtualMemory from array of bytes and offset

    template<typename TStructure> TStructure ReadVirtualMemory(HANDLE processHandle, uintptr_t baseAddress);

    uintptr_t GetAddress(HANDLE processHandle, std::vector<uintptr_t> pointers);
    uintptr_t GetAddress(HANDLE processHandle, uintptr_t baseAddress, std::vector<uintptr_t> offsets);
    uintptr_t GetAddress(HANDLE processHandle, const std::wstring& moduleName, uintptr_t offset);
    uintptr_t GetAddress(HANDLE processHandle, const std::wstring& moduleName, std::vector<uintptr_t> offsets);
    // todo GetAddress from array of bytes and offset

}