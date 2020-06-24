#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class GetProcessImageFileNameException : WinAPIException {
    public:
        GetProcessImageFileNameException(const std::string& errorMessage, DWORD lastError);
    };
}

