#include "VirtualProtectExException.h"

namespace MemoryCommando::Exceptions {

    VirtualProtectExException::VirtualProtectExException(const std::string& errorMessage, DWORD lastError): WinAPIException(errorMessage, lastError) {
    }
}
