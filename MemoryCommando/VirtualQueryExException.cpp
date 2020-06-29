#include "VirtualQueryExException.h"

namespace MemoryCommando::Exceptions {

    VirtualQueryExException::VirtualQueryExException(const std::string& errorMessage, DWORD lastError): WinAPIException(errorMessage, lastError) {
    }
}
