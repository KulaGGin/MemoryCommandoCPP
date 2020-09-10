#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class ReadProcessMemoryException : public WinAPIException {
    public:
        ReadProcessMemoryException(const std::string& errorMessage, DWORD lastError);
    };
}

