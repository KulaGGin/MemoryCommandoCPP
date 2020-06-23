#include "WinAPIException.h"

#include <utility>

namespace MemoryCommando::Exceptions {
    WinAPIException::WinAPIException(std::string errorMessage, DWORD lastError)
    : BaseException(std::move(errorMessage)), LastError(lastError) {
    }
}
