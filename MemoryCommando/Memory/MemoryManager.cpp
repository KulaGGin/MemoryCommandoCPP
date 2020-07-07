#include "MemoryManager.h"

namespace MemoryCommando::Memory {
    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(uintptr_t baseAddress) {
        std::vector<BYTE> byteSequence = ReadVirtualMemory(baseAddress, sizeof(TStructure));
        BYTE* bytePointer = &byteSequence[0];
        auto structure = reinterpret_cast<TStructure>(bytePointer);
        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(std::vector<uintptr_t> pointers) {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets) {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, offsets);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(std::wstring moduleName, uintptr_t offset) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(uintptr_t baseAddress, TStructure structure) {
        const size_t structureSize = sizeof(TStructure);
        std::vector<BYTE> structureByteSequence{};

        for(BYTE* bytePointer = &structure; bytePointer < &structure + structureSize; ++bytePointer) {
            structureByteSequence.push_back(*bytePointer);
        }

        WriteVirtualMemory(baseAddress, structureByteSequence);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(std::vector<uintptr_t> pointers, TStructure structure) {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> pointers, TStructure structure) {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, pointers);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, TStructure structure) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, TStructure structure) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        WriteVirtualMemory(calculatedAddress, structure);
    }
}
