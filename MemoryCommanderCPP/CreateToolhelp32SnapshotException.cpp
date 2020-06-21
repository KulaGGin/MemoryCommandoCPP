#include "CreateToolhelp32SnapshotException.h"

#include <utility>

namespace MemoryCommanderCpp::Exceptions {

    CreateToolhelp32SnapshotException::CreateToolhelp32SnapshotException(std::string errorMessage, DWORD lastError)
    : WinAPIException(errorMessage, lastError) {
    }

    const char* CreateToolhelp32SnapshotException::what() const {
        return ErrorMessage.c_str();
    }

    DWORD CreateToolhelp32SnapshotException::GetError() const {
        return _lastError;
    }
}
