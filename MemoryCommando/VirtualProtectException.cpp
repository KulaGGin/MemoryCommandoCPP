#include "VirtualProtectException.h"

namespace MemoryCommando::Exceptions {

    VirtualProtectException::VirtualProtectException(const std::string& errorMessage, DWORD lastError): WinAPIException(errorMessage, lastError) {
    }
}
