#pragma once

#include "BytePatternScanner.h"
#include "MemoryManager.h"
#include <winnt.h>

namespace MemoryCommandoTests {
    class MemoryScannerTests;
}

namespace MemoryCommando::External {
    using namespace Scan;
    class MemoryScanner {
    public:
        MemoryScanner(MemoryManager& memoryManager);
        std::vector<uintptr_t> Scan(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::string& pattern);
    private:
        MemoryManager& _memoryManager;
        BytePatternScanner _bytePatternScanner;
        DWORD _memoryFilter = PAGE_NOACCESS & PAGE_GUARD;

        std::vector<MEMORY_BASIC_INFORMATION> GetMemoryRegions(uintptr_t startAddress, uintptr_t endAddress);
        std::vector<BYTE> GetMemoryRegionBytes(const _MEMORY_BASIC_INFORMATION& memoryRegion, uintptr_t scanStartAddress, uintptr_t scanEndAddress);

        friend class MemoryCommandoTests::MemoryScannerTests;
    };
}

