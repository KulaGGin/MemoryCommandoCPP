#pragma once
#include <apiquery2.h>
#include <string>
#include <TlHelp32.h>
#include <vector>

namespace MemoryCommando::External {
    std::vector<PROCESSENTRY32W> GetRunningProcesses();

    PROCESSENTRY32W GetProcess(DWORD processId);
    PROCESSENTRY32W GetProcess(HANDLE processHandle);
    PROCESSENTRY32W GetProcess(const std::wstring& processName, size_t processNumber = 1);

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

    size_t GetModuleSize(DWORD processId, const std::wstring& moduleName);
    size_t GetModuleSize(const std::wstring& moduleName, HANDLE processHandle);
    size_t GetModuleSize(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber = 1);

    uintptr_t AllocateVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, size_t allocationSize, DWORD allocationType = MEM_RESERVE | MEM_COMMIT, DWORD protectionType = PAGE_EXECUTE_READWRITE);
    void FreeVirtualMemory(HANDLE processHandle, uintptr_t address, DWORD freeType = MEM_RELEASE, size_t size = 0);
    void ProtectVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, size_t protectionSize, DWORD protectionType = PAGE_EXECUTE_READWRITE);
    MEMORY_BASIC_INFORMATION QueryVirtualMemory(HANDLE processHandle, uintptr_t baseAddress);
    
    std::vector<BYTE> ReadVirtualMemory(HANDLE processHandle, uintptr_t address, int bytesNumber);
    std::vector<BYTE> ReadVirtualMemory(HANDLE processHandle, std::vector<uintptr_t> pointers, int bytesNumber);
    std::vector<BYTE> ReadVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, std::vector<uintptr_t> offsets, int bytesNumber);
    std::vector<BYTE> ReadVirtualMemory(HANDLE processHandle, const std::wstring& moduleName, uintptr_t offset, int bytesNumber);
    std::vector<BYTE> ReadVirtualMemory(HANDLE processHandle, std::wstring moduleName, std::vector<uintptr_t> offsets, int bytesNumber);
    // todo ReadVirtualMemory from array of bytes and offset

    template<typename TStructure> TStructure ReadVirtualMemory(HANDLE processHandle, uintptr_t baseAddress);
    template<typename TStructure> TStructure ReadVirtualMemory(HANDLE processHandle, std::vector<uintptr_t> pointers);
    template<typename TStructure> TStructure ReadVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, std::vector<uintptr_t> offsets);
    template<typename TStructure> TStructure ReadVirtualMemory(HANDLE processHandle, const std::wstring& moduleName, uintptr_t offset);
    template<typename TStructure> TStructure ReadVirtualMemory(HANDLE processHandle, std::wstring moduleName, std::vector<uintptr_t> offsets);

    void WriteVirtualMemory(HANDLE processHandle, uintptr_t address, const std::vector<byte>& byteSequence);
    void WriteVirtualMemory(HANDLE processHandle, std::vector<uintptr_t> pointers, std::vector<byte> byteSequence);
    void WriteVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, std::vector<uintptr_t> offsets, std::vector<byte> byteSequence);
    void WriteVirtualMemory(HANDLE processHandle, std::wstring moduleName, uintptr_t offset, std::vector<byte> byteSequence);
    void WriteVirtualMemory(HANDLE processHandle, const std::wstring& moduleName, std::vector<uintptr_t> offsets, std::vector<byte> byteSequence);

    template<typename TStructure> void WriteVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, TStructure structure);
    template<typename TStructure> void WriteVirtualMemory(HANDLE processHandle, std::vector<uintptr_t> pointers, TStructure structure);
    template<typename TStructure> void WriteVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, std::vector<uintptr_t> pointers, TStructure structure);
    template<typename TStructure> void WriteVirtualMemory(HANDLE processHandle, std::wstring moduleName, uintptr_t offset, TStructure structure);
    template<typename TStructure> void WriteVirtualMemory(HANDLE processHandle, std::wstring moduleName, std::vector<uintptr_t> offsets, TStructure structure);

    uintptr_t GetAddress(HANDLE processHandle, std::vector<uintptr_t> pointers);
    uintptr_t GetAddress(HANDLE processHandle, uintptr_t baseAddress, std::vector<uintptr_t> offsets);
    uintptr_t GetAddress(HANDLE processHandle, const std::wstring& moduleName, uintptr_t offset);
    uintptr_t GetAddress(HANDLE processHandle, const std::wstring& moduleName, std::vector<uintptr_t> offsets);
    // todo GetAddress from array of bytes and offset

}
