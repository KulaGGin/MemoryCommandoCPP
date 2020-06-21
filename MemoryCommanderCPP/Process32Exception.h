#pragma once
#include "WinAPIException.h"

namespace MemoryCommanderCpp::Exceptions {
    class Process32Exception : WinAPIException {

    public:
        Process32Exception(const std::string& errorMessage, DWORD lastError);
    };
}

