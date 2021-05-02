#include "VirtualQueryException.h"

namespace MemoryCommando::Exceptions {

    VirtualQueryException::VirtualQueryException(const std::string& errorMessage, DWORD lastError, uintptr_t address):
    WinAPIException(errorMessage, lastError),
    Address(address)
    {
    }
}
