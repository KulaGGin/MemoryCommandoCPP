#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualProtectException : public WinAPIException {
    public:
        VirtualProtectException(const std::string& errorMessage, DWORD lastError);
    };
}

