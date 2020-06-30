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

        PROCESSENTRY32W GetProcess();
        DWORD GetProcessId();
        SYSTEM_INFO GetSystemInfo();
        HANDLE GetProcessHandle();
        std::wstring GetProcessName();
        std::vector<MODULEENTRY32W> GetModules() const;
        MODULEENTRY32W GetModule(const std::wstring& moduleName);
        uintptr_t GetModuleBaseAddress(const std::wstring& moduleName);
        size_t GetModuleSize(const std::wstring& moduleName);
        uintptr_t AllocateVirtualMemory(uintptr_t baseAddress, size_t allocationSize, DWORD allocationType = MEM_RESERVE | MEM_COMMIT, DWORD protectionType = PAGE_EXECUTE_READWRITE);
        void FreeVirtualMemory(uintptr_t baseAddress, DWORD freeType = MEM_RELEASE, size_t size = 0);
        void ProtectVirtualMemory(uintptr_t baseAddress, size_t protectionSize, DWORD protectionType = PAGE_EXECUTE_READWRITE);
        MEMORY_BASIC_INFORMATION QueryVirtualMemory(uintptr_t baseAddress);

        std::vector<BYTE> ReadVirtualMemory(uintptr_t baseAddress, size_t bytesNumber);
        std::vector<BYTE> ReadVirtualMemory(std::vector<uintptr_t> pointers, size_t bytesNumber);
        std::vector<BYTE> ReadVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets, int bytesNumber);
        std::vector<BYTE> ReadVirtualMemory(std::wstring moduleName, uintptr_t offset, size_t bytesNumber);
        std::vector<BYTE> ReadVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, size_t bytesNumber);
        template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress);
        template<typename TStructure> TStructure ReadVirtualMemory(std::vector<uintptr_t> pointers);
        template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets);
        template<typename TStructure> TStructure ReadVirtualMemory(std::wstring moduleName, uintptr_t offset);
        template<typename TStructure> TStructure ReadVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets);

        void WriteVirtualMemory(uintptr_t baseAddress, std::vector<byte> byteSequence);
        void WriteVirtualMemory(std::vector<uintptr_t> pointers, std::vector<byte> byteSequence);
        void WriteVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets, std::vector<byte> byteSequence);
        void WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, std::vector<byte> byteSequence);
        void WriteVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offset, std::vector<byte> byteSequence);
        template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, TStructure structure);
        template<typename TStructure> void WriteVirtualMemory(std::vector<uintptr_t> pointers, TStructure structure);
        template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> pointers, TStructure structure);
        template<typename TStructure> void WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, TStructure structure);
        template<typename TStructure> void WriteVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, TStructure structure);

        uintptr_t GetAddress(std::vector<uintptr_t> pointers);
        uintptr_t GetAddress(uintptr_t baseAddress, std::vector<uintptr_t> pointers);
        uintptr_t GetAddress(std::wstring moduleName, uintptr_t offset);
        uintptr_t GetAddress(std::wstring moduleName, std::vector<uintptr_t> offsets);
        // todo GetAddress from array of bytes and offset but add it to the MemoryCommander probably.

        // todo Commit, Reserve, Free memory functions that auto free memory on object destruction.
    private:
        PROCESSENTRY32W _process;
        HANDLE _processHandle = nullptr;

        friend class MemoryCommandoTests::MemoryManagerExternalTests;
    };
}
