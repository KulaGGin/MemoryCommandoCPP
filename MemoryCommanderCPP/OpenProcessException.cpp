#include "OpenProcessException.h"

namespace MemoryCommanderCpp::Exceptions {
    OpenProcessException::OpenProcessException(const std::string& errorMessage, DWORD lastError)
        : WinAPIException(errorMessage, lastError) {
    }
}
