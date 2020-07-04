#include "VirtualAllocException.h"

namespace MemoryCommando::Exceptions {
    VirtualAllocException::VirtualAllocException(const std::string& errorMessage, DWORD lastError): WinAPIException(errorMessage, lastError) {
    }
}
