#include "ReadProcessMemoryException.h"

namespace MemoryCommando::Exceptions {

    ReadProcessMemoryException::ReadProcessMemoryException(const std::string& errorMessage, DWORD lastError): WinAPIException(errorMessage, lastError) {
    }
}
