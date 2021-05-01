#pragma once
#include "../MemoryManager.h"

namespace MemoryCommandoTests {
    class MemoryManagerInternalTests;
}

namespace MemoryCommando::Memory::Internal {
    class MemoryManagerInternal final : public MemoryManager {
    public:
        MemoryManagerInternal();
        uintptr_t AllocateVirtualMemory(uintptr_t baseAddress, size_t allocationSize, DWORD allocationType = MEM_RESERVE | MEM_COMMIT, DWORD protectionType = PAGE_EXECUTE_READWRITE) const override;
        void FreeVirtualMemory(uintptr_t address, DWORD freeType = MEM_RELEASE, size_t size = 0) const override;
        void ProtectVirtualMemory(uintptr_t baseAddress, size_t protectionSize, DWORD protectionType = PAGE_EXECUTE_READWRITE) const override;
        MEMORY_BASIC_INFORMATION QueryVirtualMemory(uintptr_t baseAddress) const override;
        std::vector<BYTE> ReadVirtualMemory(uintptr_t address, size_t bytesNumber) const override;
        void WriteVirtualMemory(uintptr_t address, const std::vector<BYTE>& byteSequence) const override;

        friend class MemoryCommandoTests::MemoryManagerInternalTests;
    };
}

