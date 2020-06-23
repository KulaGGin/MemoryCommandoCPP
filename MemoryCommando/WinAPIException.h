#pragma once
#include <windows.h>

#include "BaseException.h"

namespace MemoryCommanderCpp::Exceptions {
    class WinAPIException : BaseException {
    protected:
        DWORD LastError;
        std::string ErrorMessage;
    public:
        WinAPIException(std::string errorMessage, DWORD lastError);
    };
}

