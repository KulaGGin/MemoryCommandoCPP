#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualFreeExException : WinAPIException {
    public:
        VirtualFreeExException(const std::string& errorMessage, DWORD lastError);
    };
}

