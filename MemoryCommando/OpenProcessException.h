#pragma once
#include "WinAPIException.h"

namespace MemoryCommanderCpp::Exceptions {
    class OpenProcessException : WinAPIException {
    public:
        OpenProcessException(const std::string& errorMessage, DWORD lastError);
    };
}

