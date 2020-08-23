#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class GetProcessImageFileNameException : public WinAPIException {
    public:
        GetProcessImageFileNameException(const std::string& errorMessage, DWORD lastError);
    };
}

