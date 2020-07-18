#pragma once
#include <windows.h>


#include "BaseException.h"

namespace MemoryCommando::Exceptions {
    // todo inherit from WinAPIException public
    class WinAPIException : public BaseException {
    public:
        DWORD GetErrorCode();
    protected:
        DWORD _lastErrorCode;
        std::string _errorMessage;
    public:
        WinAPIException(std::string errorMessage, DWORD lastError);
    };
}

