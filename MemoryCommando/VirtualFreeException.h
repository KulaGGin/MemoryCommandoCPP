#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualFreeException : MemoryCommando::Exceptions::WinAPIException {
    public:
        VirtualFreeException(const std::string& errorMessage, DWORD lastError);
    };
}

