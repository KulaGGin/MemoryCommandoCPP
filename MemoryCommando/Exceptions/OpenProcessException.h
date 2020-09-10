#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class OpenProcessException : public WinAPIException {
    public:
        OpenProcessException(const std::string& errorMessage, DWORD lastError);
    };
}

