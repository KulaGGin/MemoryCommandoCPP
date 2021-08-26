#pragma once

#include <string>

#include "WinAPIException.h"

namespace MemoryCommando::Exceptions {
    class CreateToolhelp32SnapshotException : public WinAPIException {
    public:
        CreateToolhelp32SnapshotException(std::string errorMessage, DWORD lastError);
        const char* what() const override;
        DWORD GetError() const;
    };
}

