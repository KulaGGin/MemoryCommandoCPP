#pragma once
#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>


namespace MemoryCommandoTests {
    class MemoryManagerExternalTests;
}

namespace MemoryCommando::External {

    class MemoryManager {
    public:
        MemoryManager(DWORD processId, DWORD processAccess = PROCESS_ALL_ACCESS);
        MemoryManager(const std::wstring& processName, size_t processNumber = 1, DWORD processAccess = PROCESS_ALL_ACCESS);
        explicit MemoryManager(const std::string& processName, size_t processNumber = 1, DWORD processAccess = PROCESS_ALL_ACCESS);

        std::vector<MODULEENTRY32W> GetModules() const;
        MODULEENTRY32W GetModule(const std::wstring& moduleName);
        uintptr_t GetModuleBaseAddress(const std::wstring& moduleName);
        size_t GetModuleSize(const std::wstring& moduleName);
        PROCESSENTRY32W GetProcess();
        uintptr_t AllocateVirtualMemory(uintptr_t baseAddress, size_t allocationSize, DWORD protectionType);
        uintptr_t AllocateVirtualMemory(size_t allocationSize);
        void FreeVirtualMemory(uintptr_t baseAddress);
        DWORD ProtectVirtualMemory(uintptr_t baseAddress, size_t protectionSize, DWORD protectionType);
        MEMORY_BASIC_INFORMATION QueryVirtualMemory(uintptr_t baseAddress);

        std::vector<BYTE> ReadVirtualMemory(uintptr_t baseAddress, size_t sequenceSize);
        std::vector<BYTE> ReadVirtualMemory(std::vector<uintptr_t> pointers, size_t sequenceSize);
        std::vector<BYTE> ReadVirtualMemory(std::wstring moduleName, uintptr_t pointer, size_t sequenceSize);
        std::vector<BYTE> ReadVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> pointers, size_t sequenceSize);
        template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress);
        template<typename TStructure> TStructure ReadVirtualMemory(std::vector<uintptr_t> pointers);
        template<typename TStructure> TStructure ReadVirtualMemory(std::wstring moduleName, uintptr_t offset);
        template<typename TStructure> TStructure ReadVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets);

        void WriteVirtualMemory(uintptr_t baseAddress, std::vector<byte> byteSequence);
        void WriteVirtualMemory(std::vector<uintptr_t> pointers, std::vector<byte> byteSequence);
        void WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, std::vector<byte> byteSequence);
        void WriteVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offset, std::vector<byte> byteSequence);
        template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, TStructure structure);
        template<typename TStructure> void WriteVirtualMemory(std::vector<uintptr_t> pointers, TStructure structure);
        template<typename TStructure> void WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, TStructure structure);
        template<typename TStructure> void WriteVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, TStructure structure);

        uintptr_t GetAddress(std::vector<uintptr_t> pointers);
        uintptr_t GetAddress(uintptr_t baseAddress, std::vector<uintptr_t> pointers);
        uintptr_t GetAddress(std::wstring moduleName, uintptr_t offset);
        uintptr_t GetAddress(std::wstring moduleName, std::vector<uintptr_t> offsets);
        // todo GetAddress from array of bytes and offset but add it to the MemoryCommander probably.
    private:
        HANDLE _processHandle = nullptr;
        DWORD _processId{};
        std::wstring _processName;

        friend class MemoryCommandoTests::MemoryManagerExternalTests;
    };
}
