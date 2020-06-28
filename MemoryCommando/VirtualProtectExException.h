#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualProtectExException : WinAPIException {
    public:
        VirtualProtectExException(const std::string& errorMessage, DWORD lastError);
    };
}

