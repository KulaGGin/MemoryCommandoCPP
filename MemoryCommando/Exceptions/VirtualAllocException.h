#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualAllocException : public WinAPIException {
    public:
        VirtualAllocException(const std::string& errorMessage, DWORD lastError,
            uintptr_t baseAddress, size_t allocationSize, DWORD allocationType, DWORD protectionType);

        const uintptr_t BaseAddress;
        const size_t AllocationSize;
        const DWORD AllocationType;
        const DWORD ProtectionType;
    };
}

