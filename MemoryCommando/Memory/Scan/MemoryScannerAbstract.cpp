#include "MemoryScannerAbstract.h"

namespace MemoryCommando::Memory {
    MemoryScannerAbstract::MemoryScannerAbstract() {
        _minimumApplicationAddress = uintptr_t(HelperMethods::GetSystemInfo().lpMinimumApplicationAddress);
        _maximumApplicationAddress = uintptr_t(HelperMethods::GetSystemInfo().lpMaximumApplicationAddress);
    }
}


