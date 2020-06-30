#include "Memory.h"

namespace MemoryCommando::Memory {
    SYSTEM_INFO GetSystemInfo() {
        SYSTEM_INFO systemInfo;
        ::GetSystemInfo(&systemInfo);
        return systemInfo;
    }
}