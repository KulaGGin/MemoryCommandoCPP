#pragma once
#include "WinAPIException.h"

#include <string>

namespace MemoryCommanderCpp::Exceptions {
    class CreateToolhelp32SnapshotException : WinAPIException {
    private:
        DWORD _lastError;
    public:
        CreateToolhelp32SnapshotException(std::string errorMessage, DWORD lastError);
        const char* what() const override;
        DWORD GetError() const;
    };
}

