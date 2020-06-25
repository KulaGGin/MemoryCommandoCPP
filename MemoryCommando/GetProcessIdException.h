#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class GetProcessIdException : WinAPIException {
    public:
        GetProcessIdException(const std::string& errorMessage, DWORD lastError);
    };
}

