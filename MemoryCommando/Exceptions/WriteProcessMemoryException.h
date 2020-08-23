#pragma once
#include "WinAPIException.h"


namespace MemoryCommando::Exceptions {
    class WriteProcessMemoryException : public WinAPIException {
    public:
        WriteProcessMemoryException(const std::string& errorMessage, DWORD lastError);
    };
}

