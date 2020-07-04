#include "GetProcessImageFileNameException.h"

namespace MemoryCommando::Exceptions {
    GetProcessImageFileNameException::GetProcessImageFileNameException(const std::string& errorMessage, DWORD lastError): WinAPIException(errorMessage, lastError) {
    }
}
