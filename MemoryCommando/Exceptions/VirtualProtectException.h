#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualProtectException : public WinAPIException {
    public:
        VirtualProtectException(const std::string& errorMessage, DWORD lastError, uintptr_t baseAddress, size_t protectionSize, DWORD protectionType);

        const uintptr_t Address;
        const size_t ProtectionSize;
        const DWORD ProtectionType;
    };
}

