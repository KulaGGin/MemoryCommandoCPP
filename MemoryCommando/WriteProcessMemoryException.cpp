#include "WriteProcessMemoryException.h"

namespace MemoryCommando::Exceptions {

    WriteProcessMemoryException::WriteProcessMemoryException(const std::string& errorMessage, DWORD lastError): WinAPIException(errorMessage, lastError) {
    }
}
