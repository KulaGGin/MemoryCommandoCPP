#pragma once
#include <string>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>

namespace MemoryCommando::Internal {
    PROCESSENTRY32W GetProcess();
    DWORD GetProcessId();
    HANDLE GetProcessHandle();
    std::wstring GetProcessName();
    std::vector<MODULEENTRY32W> GetModules();
    MODULEENTRY32W GetModule(const std::string& moduleName);
    uintptr_t GetModuleBaseAddress(const std::wstring& moduleName);
    size_t GetModuleSize(const std::wstring& moduleName);

    uintptr_t AllocateVirtualMemory(uintptr_t baseAddress, size_t allocationSize, DWORD allocationType = MEM_RESERVE | MEM_COMMIT, DWORD protectionType = PAGE_EXECUTE_READWRITE);
    void FreeVirtualMemory(uintptr_t address, DWORD freeType = MEM_RELEASE, size_t size = 0);
    void ProtectVirtualMemory(uintptr_t baseAddress, size_t protectionSize, DWORD protectionType = PAGE_EXECUTE_READWRITE);
    MEMORY_BASIC_INFORMATION QueryVirtualMemory(uintptr_t baseAddress);

    template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress);
    template<typename TStructure> TStructure ReadVirtualMemory(std::vector<uintptr_t> pointers);
    template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets);
    template<typename TStructure> TStructure ReadVirtualMemory(const std::wstring& moduleName, uintptr_t offset);
    template<typename TStructure> TStructure ReadVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets);
    // todo ReadVirtualMemory from array of bytes and offset

    void WriteVirtualMemory(uintptr_t address, const std::vector<byte>& byteSequence);
    void WriteVirtualMemory(std::vector<uintptr_t> pointers, std::vector<byte> byteSequence);
    void WriteVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets, std::vector<byte> byteSequence);
    void WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, std::vector<byte> byteSequence);
    void WriteVirtualMemory(const std::wstring& moduleName, std::vector<uintptr_t> offsets, std::vector<byte> byteSequence);
    // todo WriteVirtualMemory from array of bytes and offset

    template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, TStructure structure);
    template<typename TStructure> void WriteVirtualMemory(std::vector<uintptr_t> pointers, TStructure structure);
    template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> pointers, TStructure structure);
    template<typename TStructure> void WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, TStructure structure);
    template<typename TStructure> void WriteVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, TStructure structure);
    // todo WriteVirtualMemory from array of bytes and offset

    uintptr_t GetAddress(std::vector<uintptr_t> pointers);
    uintptr_t GetAddress(uintptr_t baseAddress, std::vector<uintptr_t> offsets);
    uintptr_t GetAddress(const std::wstring& moduleName, uintptr_t offset);
    uintptr_t GetAddress(const std::wstring& moduleName, std::vector<uintptr_t> offsets);
    // todo GetAddress from array of bytes and offset
}

