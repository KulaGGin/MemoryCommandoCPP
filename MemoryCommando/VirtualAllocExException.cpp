#include "VirtualAllocExException.h"

namespace MemoryCommando::Exceptions {
    VirtualAllocExException::VirtualAllocExException(const std::string& errorMessage, DWORD lastError): WinAPIException(errorMessage, lastError) {
    }
}
