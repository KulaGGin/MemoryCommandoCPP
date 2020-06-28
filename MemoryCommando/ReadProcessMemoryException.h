#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class ReadProcessMemoryException : WinAPIException {
    public:
        ReadProcessMemoryException(const std::string& errorMessage, DWORD lastError);
    };
}

