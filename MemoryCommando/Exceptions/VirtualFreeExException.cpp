#include "VirtualFreeExException.h"

namespace MemoryCommando::Exceptions {
    VirtualFreeExException::VirtualFreeExException(const std::string& errorMessage, DWORD lastError): WinAPIException(errorMessage, lastError) {
    }
}
