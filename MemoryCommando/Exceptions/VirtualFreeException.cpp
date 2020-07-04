#include "VirtualFreeException.h"

namespace MemoryCommando::Exceptions {
    VirtualFreeException::VirtualFreeException(const std::string& errorMessage, DWORD lastError): WinAPIException(errorMessage, lastError) {
    }
}
