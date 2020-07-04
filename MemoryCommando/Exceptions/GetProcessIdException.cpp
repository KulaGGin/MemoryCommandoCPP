#include "GetProcessIdException.h"

namespace MemoryCommando::Exceptions {
    GetProcessIdException::GetProcessIdException(const std::string& errorMessage, DWORD lastError): WinAPIException(errorMessage, lastError) {
    }
}
