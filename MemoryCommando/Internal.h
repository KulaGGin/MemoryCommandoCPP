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
    MODULEENTRY32W GetModule(const std::wstring& moduleName);
    uintptr_t GetModuleBaseAddress(const std::wstring& moduleName);
    size_t GetModuleSize(const std::wstring& moduleName);

    uintptr_t AllocateVirtualMemory(uintptr_t baseAddress, size_t allocationSize, DWORD allocationType = MEM_RESERVE | MEM_COMMIT, DWORD protectionType = PAGE_EXECUTE_READWRITE);
    void FreeVirtualMemory(uintptr_t address, DWORD freeType = MEM_RELEASE, size_t size = 0);
    void ProtectVirtualMemory(uintptr_t baseAddress, size_t protectionSize, DWORD protectionType = PAGE_EXECUTE_READWRITE);
    MEMORY_BASIC_INFORMATION QueryVirtualMemory(uintptr_t baseAddress);

    std::vector<BYTE> ReadVirtualMemory(uintptr_t address, int bytesNumber);
    std::vector<BYTE> ReadVirtualMemory(const std::vector<uintptr_t>& pointers, int bytesNumber);
    std::vector<BYTE> ReadVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, int bytesNumber);
    std::vector<BYTE> ReadVirtualMemory(const std::wstring& moduleName, uintptr_t offset, int bytesNumber);
    std::vector<BYTE> ReadVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, int bytesNumber);
    // todo ReadVirtualMemory from array of bytes and offset

    template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress);
    template<typename TStructure> TStructure ReadVirtualMemory(std::vector<uintptr_t> pointers);
    template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets);
    template<typename TStructure> TStructure ReadVirtualMemory(const std::wstring& moduleName, uintptr_t offset);
    template<typename TStructure> TStructure ReadVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets);
    // todo ReadVirtualMemory from array of bytes and offset

    void WriteVirtualMemory(uintptr_t address, const std::vector<byte>& byteSequence);
    void WriteVirtualMemory(const std::vector<uintptr_t>& pointers, const std::vector<byte>& byteSequence);
    void WriteVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, const std::vector<byte>& byteSequence);
    void WriteVirtualMemory(const std::wstring& moduleName, uintptr_t offset, const std::vector<byte>& byteSequence);
    void WriteVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const std::vector<byte>& byteSequence);
    // todo WriteVirtualMemory from array of bytes and offset

    template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, const TStructure& structure);
    template<typename TStructure> void WriteVirtualMemory(const std::vector<uintptr_t>& pointers, const TStructure& structure);
    template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t>& pointers, const TStructure& structure);
    template<typename TStructure> void WriteVirtualMemory(const std::wstring& moduleName, uintptr_t offset, const TStructure& structure);
    template<typename TStructure> void WriteVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const TStructure& structure);
    // todo WriteVirtualMemory from array of bytes and offset

    uintptr_t GetAddress(const std::vector<uintptr_t>& pointers);
    uintptr_t GetAddress(uintptr_t baseAddress, std::vector<uintptr_t> offsets);
    uintptr_t GetAddress(const std::wstring& moduleName, uintptr_t offset);
    uintptr_t GetAddress(const std::wstring& moduleName, std::vector<uintptr_t> offsets);
    // todo GetAddress from array of bytes and offset
}

