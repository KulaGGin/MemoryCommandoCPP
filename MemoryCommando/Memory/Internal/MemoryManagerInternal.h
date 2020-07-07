#pragma once
#include "../MemoryManager.h"

namespace MemoryCommando::Memory::Internal {
    class MemoryManagerInternal final : public MemoryManager {
    public:
        MemoryManagerInternal();
        DWORD GetProcessId() override;
        HANDLE GetProcessHandle() override;
        uintptr_t AllocateVirtualMemory(uintptr_t baseAddress, size_t allocationSize, DWORD allocationType, DWORD protectionType) override;
        void FreeVirtualMemory(uintptr_t address, DWORD freeType, size_t size) override;
        void ProtectVirtualMemory(uintptr_t baseAddress, size_t protectionSize, DWORD protectionType) override;
        MEMORY_BASIC_INFORMATION QueryVirtualMemory(uintptr_t baseAddress) override;
        std::vector<BYTE> ReadVirtualMemory(uintptr_t address, size_t bytesNumber) override;
        void WriteVirtualMemory(uintptr_t address, const std::vector<byte>& byteSequence) override;
    };
}

