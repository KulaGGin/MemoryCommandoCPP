#include "VirtualAllocException.h"

namespace MemoryCommando::Exceptions {
    VirtualAllocException::VirtualAllocException(const std::string& errorMessage, DWORD lastError,
                                                 uintptr_t baseAddress, size_t allocationSize, DWORD allocationType,
                                                 DWORD protectionType): WinAPIException(errorMessage, lastError),
                                                                        BaseAddress(baseAddress),
                                                                        AllocationSize(allocationSize),
                                                                        AllocationType(allocationType),
                                                                        ProtectionType(protectionType) {
    }
}
