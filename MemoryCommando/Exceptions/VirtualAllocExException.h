#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualAllocExException : public WinAPIException {
    public:
        VirtualAllocExException(const std::string& errorMessage, DWORD lastError);
    };
}

