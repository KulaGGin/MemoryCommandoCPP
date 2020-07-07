#pragma once
#include "../MemoryManager.h"

namespace MemoryCommando::Memory::Internal {
    class MemoryManagerInternal final : public MemoryManager {
    public:
        MemoryManagerInternal();
        DWORD GetProcessId() override;
        HANDLE GetProcessHandle() override;
        uintptr_t AllocateVirtualMemory(uintptr_t baseAddress, size_t allocationSize, DWORD allocationType = MEM_RESERVE | MEM_COMMIT, DWORD protectionType = PAGE_EXECUTE_READWRITE) override;
        void FreeVirtualMemory(uintptr_t address, DWORD freeType = MEM_RELEASE, size_t size = 0) override;
        void ProtectVirtualMemory(uintptr_t baseAddress, size_t protectionSize, DWORD protectionType = PAGE_EXECUTE_READWRITE) override;
        MEMORY_BASIC_INFORMATION QueryVirtualMemory(uintptr_t baseAddress) override;
        std::vector<BYTE> ReadVirtualMemory(uintptr_t address, size_t bytesNumber) override;
        void WriteVirtualMemory(uintptr_t address, const std::vector<byte>& byteSequence) override;
    };
}

