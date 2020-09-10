#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class GetProcessIdException : public WinAPIException {
    public:
        GetProcessIdException(const std::string& errorMessage, DWORD lastError);
    };
}

