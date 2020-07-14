#pragma once
#include <windows.h>


#include "BaseException.h"

namespace MemoryCommando::Exceptions {
    // todo inherit from WinAPIException public
    class WinAPIException : public BaseException {
    protected:
        DWORD LastError;
        std::string ErrorMessage;
    public:
        WinAPIException(std::string errorMessage, DWORD lastError);
    };
}

