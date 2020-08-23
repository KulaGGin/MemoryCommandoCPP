#pragma once
#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class Process32Exception : public WinAPIException {

    public:
        Process32Exception(const std::string& errorMessage, DWORD lastError);
    };
}

