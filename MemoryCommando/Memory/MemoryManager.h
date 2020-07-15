#pragma once
#include <string>
#include <vector>




#include "windows.h"
#include "TlHelp32.h"
#include <wil/resource.h>

namespace MemoryCommando::Memory {
    class MemoryManager {
    public:
        MemoryManager() = default;
        MemoryManager(const MemoryManager&) = default;
        MemoryManager& operator=(const MemoryManager&) = default;
        virtual ~MemoryManager() = default;

        virtual PROCESSENTRY32W GetProcess() const;
        virtual std::wstring GetProcessName() const;
        virtual std::vector<MODULEENTRY32W> GetModules() const;
        virtual MODULEENTRY32W GetModule(const std::wstring& moduleName) const;
        virtual uintptr_t GetModuleBaseAddress(const std::wstring& moduleName) const;
        virtual size_t GetModuleSize(const std::wstring& moduleName) const;

        virtual DWORD GetProcessId() const = 0;
        virtual HANDLE GetProcessHandle() const = 0;

        virtual uintptr_t AllocateVirtualMemory(uintptr_t baseAddress, size_t allocationSize, DWORD allocationType = MEM_RESERVE | MEM_COMMIT, DWORD protectionType = PAGE_EXECUTE_READWRITE) const = 0;
        virtual uintptr_t AllocateVirtualMemory(uintptr_t allocationSize) const;
        virtual void FreeVirtualMemory(uintptr_t baseAddress, DWORD freeType = MEM_RELEASE, size_t size = 0) const = 0;
        virtual void ProtectVirtualMemory(uintptr_t baseAddress, size_t protectionSize, DWORD protectionType = PAGE_EXECUTE_READWRITE) const = 0;
        virtual MEMORY_BASIC_INFORMATION QueryVirtualMemory(uintptr_t baseAddress) const = 0;

        virtual std::vector<BYTE> ReadVirtualMemory(uintptr_t baseAddress, size_t bytesNumber) const = 0;
        virtual std::vector<BYTE> ReadVirtualMemory(const std::vector<uintptr_t>& pointers, size_t bytesNumber) const;
        virtual std::vector<BYTE> ReadVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, int bytesNumber) const;
        virtual std::vector<BYTE> ReadVirtualMemory(const std::wstring& moduleName, uintptr_t offset, size_t bytesNumber) const;
        virtual std::vector<BYTE> ReadVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, size_t bytesNumber) const;
        // todo ReadVirtualMemory from array of bytes and offset

        virtual void WriteVirtualMemory(uintptr_t baseAddress, const std::vector<BYTE>& byteSequence) const = 0;
        virtual void WriteVirtualMemory(const std::vector<uintptr_t>& pointers, const std::vector<BYTE>& byteSequence) const;
        virtual void WriteVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, const std::vector<BYTE>& byteSequence) const;
        virtual void WriteVirtualMemory(const std::wstring& moduleName, uintptr_t offset, const std::vector<BYTE>& byteSequence) const;
        virtual void WriteVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const std::vector<BYTE>& byteSequence) const;
        // todo WriteVirtualMemory from array of bytes and offset

        virtual uintptr_t GetAddress(const std::vector<uintptr_t>& pointers) const;
        virtual uintptr_t GetAddress(uintptr_t baseAddress, std::vector<uintptr_t> offsets) const;
        virtual uintptr_t GetAddress(const std::wstring& moduleName, uintptr_t offset) const;
        virtual uintptr_t GetAddress(const std::wstring& moduleName, std::vector<uintptr_t> offsets) const;
        // todo GetAddress from array of bytes and offset but add it to the MemoryCommander probably.

        template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress) const;
        template<typename TStructure> TStructure ReadVirtualMemory(const std::vector<uintptr_t>& pointers) const;
        template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets) const;
        template<typename TStructure> TStructure ReadVirtualMemory(const std::wstring& moduleName, uintptr_t offset) const;
        template<typename TStructure> TStructure ReadVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets) const;
        // todo ReadVirtualMemory from array of bytes and offset

        template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, const TStructure& structure) const;
        template<typename TStructure> void WriteVirtualMemory(const std::vector<uintptr_t>& pointers, const TStructure& structure) const;
        template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t>& pointers, const TStructure& structure) const;
        template<typename TStructure> void WriteVirtualMemory(const std::wstring& moduleName, uintptr_t offset, const TStructure& structure) const;
        template<typename TStructure> void WriteVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const TStructure& structure) const;
        // todo WriteVirtualMemory from array of bytes and offset

        // todo Commit, Reserve, Free memory functions that auto free memory on object destruction.

    protected:
        wil::unique_handle _processHandle = nullptr;
        DWORD _processId = 0;
        PROCESSENTRY32W _process = {};
    };

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const uintptr_t baseAddress) const {
        std::vector<BYTE> byteSequence = ReadVirtualMemory(baseAddress, sizeof(TStructure));
        TStructure* bytePointer = static_cast<TStructure*>(static_cast<void*>(&byteSequence[0]));
        TStructure structure = *bytePointer;
        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const std::vector<uintptr_t>& pointers) const {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& offsets) const {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, offsets);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const std::wstring& moduleName, const uintptr_t offset) const {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets) const {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const uintptr_t baseAddress, const TStructure& structure) const {
        const size_t structureSize = sizeof(TStructure);
        std::vector<BYTE> structureByteSequence{};
        structureByteSequence.resize(sizeof TStructure);

        std::memcpy(&structureByteSequence[0], &structure, sizeof TStructure);

        WriteVirtualMemory(baseAddress, structureByteSequence);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const std::vector<uintptr_t>& pointers, const TStructure& structure) const {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& pointers, const TStructure& structure) const {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, pointers);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const std::wstring& moduleName, const uintptr_t offset, const TStructure& structure) const {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const TStructure& structure) const {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        WriteVirtualMemory(calculatedAddress, structure);
    }
}

