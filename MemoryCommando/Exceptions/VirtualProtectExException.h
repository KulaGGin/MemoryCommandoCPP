#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualProtectExException : public WinAPIException {
    public:
        VirtualProtectExException(const std::string& errorMessage, DWORD lastError);
    };
}

