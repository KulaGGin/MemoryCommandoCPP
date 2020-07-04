#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualAllocException : WinAPIException {
    public:
        VirtualAllocException(const std::string& errorMessage, DWORD lastError);

    };
}

