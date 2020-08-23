#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualFreeExException : public WinAPIException {
    public:
        VirtualFreeExException(const std::string& errorMessage, DWORD lastError);
    };
}

