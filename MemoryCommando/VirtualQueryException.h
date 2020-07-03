#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualQueryException : WinAPIException {
    public:
        VirtualQueryException(const std::string& errorMessage, DWORD lastError);
    };
}

