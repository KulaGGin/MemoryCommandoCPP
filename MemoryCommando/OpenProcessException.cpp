#include "OpenProcessException.h"

namespace MemoryCommando::Exceptions {
    OpenProcessException::OpenProcessException(const std::string& errorMessage, DWORD lastError)
        : WinAPIException(errorMessage, lastError) {
    }
}
