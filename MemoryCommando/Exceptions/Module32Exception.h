#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class Module32Exception : public WinAPIException{
    public:
        Module32Exception(const std::string& errorMessage, DWORD lastError);
    };
}

