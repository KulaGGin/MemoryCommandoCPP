#pragma once
#include <windows.h>


#include "BaseException.h"

namespace MemoryCommando::Exceptions {
    class WinAPIException : public BaseException {
    public:
        WinAPIException(std::string errorMessage, DWORD lastError) : BaseException(std::move(errorMessage)), _lastErrorCode(lastError) {
        }
        DWORD GetErrorCode() {
            return _lastErrorCode;
        }
    protected:
        DWORD _lastErrorCode;
    };
}

