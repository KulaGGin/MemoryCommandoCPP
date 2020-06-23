#pragma once
#include "WinAPIException.h"

namespace MemoryCommanderCpp::Exceptions {
    using namespace MemoryCommanderCpp::Exceptions;
    class Module32Exception : WinAPIException{
    public:
        Module32Exception(const std::string& errorMessage, DWORD lastError);
    };
}

