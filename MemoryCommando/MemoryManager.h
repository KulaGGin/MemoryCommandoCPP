#pragma once
#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>


namespace MemoryCommandoTests {
    class MemoryManagerExternalTests;
}

namespace MemoryCommando {

    class MemoryManager {
    public:
        MemoryManager(DWORD processId, DWORD processAccess = PROCESS_ALL_ACCESS);
        MemoryManager(const std::wstring& processName, size_t processNumber = 1, DWORD processAccess = PROCESS_ALL_ACCESS);
        explicit MemoryManager(const std::string& processName, size_t processNumber = 1, DWORD processAccess = PROCESS_ALL_ACCESS);

        std::vector<MODULEENTRY32> GetModules() const;
        std::vector<HMODULE> GetModulesHandles() const;
        HMODULE GetModule(std::wstring moduleName);
        DWORD_PTR GetModuleBaseAddress(std::wstring moduleName);
        size_t GetModuleSize(std::wstring moduleName);
        SYSTEM_INFO GetProcessInfo();
        DWORD_PTR AllocateVirtualMemory(DWORD_PTR baseAddress, size_t allocationSize, DWORD protectionType);
        DWORD_PTR AllocateVirtualMemory(size_t allocationSize);
        void FreeVirtualMemory(DWORD_PTR baseAddress);
        DWORD ProtectVirtualMemory(DWORD_PTR baseAddress, size_t protectionSize, DWORD protectionType);
        MEMORY_BASIC_INFORMATION QueryVirtualMemory(DWORD_PTR baseAddress);

        std::vector<BYTE> ReadVirtualMemory(DWORD_PTR baseAddress, size_t sequenceSize);
        std::vector<BYTE> ReadVirtualMemory(std::vector<DWORD_PTR> pointers, size_t sequenceSize);
        std::vector<BYTE> ReadVirtualMemory(std::wstring moduleName, DWORD_PTR pointer, size_t sequenceSize);
        std::vector<BYTE> ReadVirtualMemory(std::wstring moduleName, std::vector<DWORD_PTR> pointers, size_t sequenceSize);
        template<typename TStructure> TStructure ReadVirtualMemory(DWORD_PTR baseAddress);
        template<typename TStructure> TStructure ReadVirtualMemory(std::vector<DWORD_PTR> pointers);
        template<typename TStructure> TStructure ReadVirtualMemory(std::wstring moduleName, DWORD_PTR offset);
        template<typename TStructure> TStructure ReadVirtualMemory(std::wstring moduleName, std::vector<DWORD_PTR> offsets);

        void WriteVirtualMemory(DWORD_PTR baseAddress, std::vector<byte> byteSequence);
        void WriteVirtualMemory(std::vector<DWORD_PTR> pointers, std::vector<byte> byteSequence);
        void WriteVirtualMemory(std::wstring moduleName, DWORD_PTR offset, std::vector<byte> byteSequence);
        void WriteVirtualMemory(std::wstring moduleName, std::vector<DWORD_PTR> offset, std::vector<byte> byteSequence);
        template<typename TStructure> void WriteVirtualMemory(DWORD_PTR baseAddress, TStructure structure);
        template<typename TStructure> void WriteVirtualMemory(std::vector<DWORD_PTR> pointers, TStructure structure);
        template<typename TStructure> void WriteVirtualMemory(std::wstring moduleName, DWORD_PTR offset, TStructure structure);
        template<typename TStructure> void WriteVirtualMemory(std::wstring moduleName, std::vector<DWORD_PTR> offsets, TStructure structure);

        DWORD_PTR GetAddress(std::vector<DWORD_PTR> pointers);
        DWORD_PTR GetAddress(DWORD_PTR baseAddress, std::vector<DWORD_PTR> pointers);
        DWORD_PTR GetAddress(std::wstring moduleName, DWORD_PTR offset);
        DWORD_PTR GetAddress(std::wstring moduleName, std::vector<DWORD_PTR> offsets);
        // todo GetAddress from array of bytes and offset but add it to the MemoryCommander probably.
    private:
        HANDLE _processHandle = nullptr;

        friend class MemoryCommandoTests::MemoryManagerExternalTests;
    };
}
