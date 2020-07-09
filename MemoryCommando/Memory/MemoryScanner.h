#pragma once

#include <vector>

#include "BytePatternScanner.h"
#include "External/MemoryManagerExternal.h"
#include <winnt.h>

namespace MemoryCommandoTests {
    class MemoryScannerTests;
}

namespace MemoryCommando::Memory {
    class MemoryScanner {
    public:
        MemoryScanner(const MemoryManager &memoryManager);
        std::vector<uintptr_t> Scan(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::string& pattern) const;
    private:
        const MemoryManager& _memoryManager;
        const BytePatternScanner _bytePatternScanner{};
        const DWORD _memoryFilter = PAGE_NOACCESS & PAGE_GUARD;

        std::vector<MEMORY_BASIC_INFORMATION> GetMemoryRegions(uintptr_t startAddress, uintptr_t endAddress) const;

        friend class MemoryCommandoTests::MemoryScannerTests;
    };
}

