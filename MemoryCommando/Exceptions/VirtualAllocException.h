#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualAllocException : public WinAPIException {
    public:
        VirtualAllocException(const std::string& errorMessage, DWORD lastError);

    };
}

