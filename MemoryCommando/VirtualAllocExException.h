#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualAllocExException : WinAPIException {
    public:
        VirtualAllocExException(const std::string& errorMessage, DWORD lastError);
    };
}

