#include "CreateToolhelp32SnapshotException.h"

#include <utility>

namespace MemoryCommando::Exceptions {
    CreateToolhelp32SnapshotException::CreateToolhelp32SnapshotException(std::string errorMessage, DWORD lastError): WinAPIException(std::move(errorMessage), lastError) {
    }

    const char* CreateToolhelp32SnapshotException::what() const {
        return _errorMessage.c_str();
    }

    DWORD CreateToolhelp32SnapshotException::GetError() const {
        return _lastErrorCode;
    }
}
