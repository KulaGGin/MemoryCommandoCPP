#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualFreeException : public WinAPIException {
    public:
        VirtualFreeException(const std::string& errorMessage, DWORD lastError);
    };
}

