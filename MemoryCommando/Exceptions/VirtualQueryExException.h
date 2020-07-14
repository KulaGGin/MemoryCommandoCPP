#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class VirtualQueryExException : public WinAPIException {
    public:
        VirtualQueryExException(const std::string& errorMessage, DWORD lastError);
    };
}

