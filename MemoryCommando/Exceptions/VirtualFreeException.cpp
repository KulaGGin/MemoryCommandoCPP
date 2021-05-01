#include "VirtualFreeException.h"

namespace MemoryCommando::Exceptions {
    VirtualFreeException::VirtualFreeException(const std::string& errorMessage, const DWORD lastError,
                                               const uintptr_t address, const DWORD freeType, const size_t size) :
        WinAPIException(errorMessage, lastError),
        Address(address),
        FreeType(freeType),
        Size(size) {
    }
}
