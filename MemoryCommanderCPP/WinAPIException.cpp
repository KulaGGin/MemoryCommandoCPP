#include "WinAPIException.h"

#include <utility>

namespace MemoryCommanderCpp::Exceptions {
    WinAPIException::WinAPIException(std::string errorMessage, DWORD lastError)
    : BaseException(std::move(errorMessage)), LastError(lastError) {
    }
}
