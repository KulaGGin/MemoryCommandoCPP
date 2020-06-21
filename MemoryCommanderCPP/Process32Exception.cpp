#include "Process32Exception.h"

namespace MemoryCommanderCpp::Exceptions {
    Process32Exception::Process32Exception(const std::string& errorMessage, DWORD lastError): WinAPIException(errorMessage, lastError) {
    }
}
