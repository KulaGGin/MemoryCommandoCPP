#pragma once

#include "../BytePatternScanner.h"
#include "MemoryManagerExternal.h"
#include <winnt.h>

namespace MemoryCommandoTests {
    class MemoryScannerTests;
}

namespace MemoryCommando::External {
    using namespace Scan;
    class MemoryScanner {
    public:
        MemoryScanner(MemoryManagerExternal& memoryManager);
        std::vector<uintptr_t> Scan(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::string& pattern);
    private:
        MemoryManagerExternal& _memoryManager;
        BytePatternScanner _bytePatternScanner;
        DWORD _memoryFilter = PAGE_NOACCESS & PAGE_GUARD;

        std::vector<MEMORY_BASIC_INFORMATION> GetMemoryRegions(uintptr_t startAddress, uintptr_t endAddress);

        friend class MemoryCommandoTests::MemoryScannerTests;
    };
}

