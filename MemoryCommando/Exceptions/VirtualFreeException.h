#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualFreeException : public WinAPIException {
    public:
        VirtualFreeException(const std::string& errorMessage, DWORD lastError, uintptr_t address, DWORD freeType, size_t size);

        const uintptr_t Address;
        const DWORD FreeType;
        const size_t Size;
    };
}

