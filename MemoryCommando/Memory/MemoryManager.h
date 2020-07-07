#pragma once
#include <string>
#include <vector>




#include "windows.h"
#include "TlHelp32.h"
#include <wil\resource.h>

namespace MemoryCommando::Memory {
    class MemoryManager {
    public:
        virtual ~MemoryManager() = default;
        virtual PROCESSENTRY32W GetProcess() = 0;
        virtual DWORD GetProcessId() = 0;
        virtual SYSTEM_INFO GetSystemInfo() = 0;
        virtual HANDLE GetProcessHandle() = 0;
        virtual std::wstring GetProcessName() = 0;
        virtual std::vector<MODULEENTRY32W> GetModules() const = 0;
        virtual MODULEENTRY32W GetModule(const std::wstring& moduleName) = 0;
        virtual uintptr_t GetModuleBaseAddress(const std::wstring& moduleName) = 0;
        virtual size_t GetModuleSize(const std::wstring& moduleName) = 0;
        virtual uintptr_t AllocateVirtualMemory(uintptr_t baseAddress, size_t allocationSize, DWORD allocationType = MEM_RESERVE | MEM_COMMIT, DWORD protectionType = PAGE_EXECUTE_READWRITE) = 0;
        virtual void FreeVirtualMemory(uintptr_t baseAddress, DWORD freeType = MEM_RELEASE, size_t size = 0) = 0;
        virtual void ProtectVirtualMemory(uintptr_t baseAddress, size_t protectionSize, DWORD protectionType = PAGE_EXECUTE_READWRITE) = 0;
        virtual MEMORY_BASIC_INFORMATION QueryVirtualMemory(uintptr_t baseAddress) = 0;

        virtual std::vector<BYTE> ReadVirtualMemory(uintptr_t baseAddress, size_t bytesNumber) = 0;
        virtual std::vector<BYTE> ReadVirtualMemory(std::vector<uintptr_t> pointers, size_t bytesNumber) = 0;
        virtual std::vector<BYTE> ReadVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets, int bytesNumber) = 0;
        virtual std::vector<BYTE> ReadVirtualMemory(std::wstring moduleName, uintptr_t offset, size_t bytesNumber) = 0;
        virtual std::vector<BYTE> ReadVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, size_t bytesNumber) = 0;
        // todo ReadVirtualMemory from array of bytes and offset

        virtual void WriteVirtualMemory(uintptr_t baseAddress, std::vector<byte> byteSequence) = 0;
        virtual void WriteVirtualMemory(std::vector<uintptr_t> pointers, std::vector<byte> byteSequence) = 0;
        virtual void WriteVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets, std::vector<byte> byteSequence) = 0;
        virtual void WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, std::vector<byte> byteSequence) = 0;
        virtual void WriteVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offset, std::vector<byte> byteSequence) = 0;
        // todo WriteVirtualMemory from array of bytes and offset

        virtual uintptr_t GetAddress(std::vector<uintptr_t> pointers) = 0;
        virtual uintptr_t GetAddress(uintptr_t baseAddress, std::vector<uintptr_t> pointers) = 0;
        virtual uintptr_t GetAddress(std::wstring moduleName, uintptr_t offset) = 0;
        virtual uintptr_t GetAddress(std::wstring moduleName, std::vector<uintptr_t> offsets) = 0;
        // todo GetAddress from array of bytes and offset but add it to the MemoryCommander probably.

        template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress);
        template<typename TStructure> TStructure ReadVirtualMemory(std::vector<uintptr_t> pointers);
        template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets);
        template<typename TStructure> TStructure ReadVirtualMemory(std::wstring moduleName, uintptr_t offset);
        template<typename TStructure> TStructure ReadVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets);
        // todo ReadVirtualMemory from array of bytes and offset

        template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, TStructure structure);
        template<typename TStructure> void WriteVirtualMemory(std::vector<uintptr_t> pointers, TStructure structure);
        template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> pointers, TStructure structure);
        template<typename TStructure> void WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, TStructure structure);
        template<typename TStructure> void WriteVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, TStructure structure);
        // todo WriteVirtualMemory from array of bytes and offset

        // todo Commit, Reserve, Free memory functions that auto free memory on object destruction.

    private:
        wil::unique_handle _processHandle;
    };
}

