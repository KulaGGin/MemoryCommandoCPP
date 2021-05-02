#include "VirtualProtectException.h"

namespace MemoryCommando::Exceptions {
    VirtualProtectException::VirtualProtectException(const std::string& errorMessage, const DWORD lastError,
                                                     const uintptr_t baseAddress, const size_t protectionSize,
                                                     const DWORD protectionType)
        : WinAPIException(errorMessage, lastError),
          Address(baseAddress),
          ProtectionSize(protectionSize),
          ProtectionType(protectionType) {
    }
}
