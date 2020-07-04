#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualProtectException : WinAPIException {
    public:
        VirtualProtectException(const std::string& errorMessage, DWORD lastError);
    };
}

