#pragma once
#include "WinAPIException.h"


namespace MemoryCommando::Exceptions {
    class WriteProcessMemoryException : WinAPIException {
    public:
        WriteProcessMemoryException(const std::string& errorMessage, DWORD lastError);
    };
}

