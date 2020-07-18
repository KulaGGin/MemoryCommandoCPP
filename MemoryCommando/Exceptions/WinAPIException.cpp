#include "WinAPIException.h"

#include <utility>

namespace MemoryCommando::Exceptions {
    DWORD WinAPIException::GetErrorCode() {
        return _lastErrorCode;
    }

    WinAPIException::WinAPIException(std::string errorMessage, DWORD lastError)
    : BaseException(std::move(errorMessage)), _lastErrorCode(lastError) {
    }
}
