#include "VirtualQueryException.h"

namespace MemoryCommando::Exceptions {

    VirtualQueryException::VirtualQueryException(const std::string& errorMessage, DWORD lastError): WinAPIException(errorMessage, lastError) {
    }
}
