#include "Module32Exception.h"

namespace MemoryCommanderCpp::Exceptions {
    Module32Exception::Module32Exception(const std::string& errorMessage, DWORD lastError)
    : WinAPIException(errorMessage, lastError) {
    }
}
