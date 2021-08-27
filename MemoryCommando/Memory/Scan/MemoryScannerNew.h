#pragma once
#include "PatternScanner.h"

namespace MemoryCommando::Memory {
    class MemoryScannerNew {
        FRIEND_TEST(MemoryScanner, GetsReadableMemoryRegions);
        FRIEND_TEST(MemoryScanner, QueriesVirtualMemory);
        FRIEND_TEST(MemoryScanner, IsMemoryRegionReadable);
        FRIEND_TEST(MemoryScanner, IsMemoryRegionUsed);
    public:
        ScanResults Scan(uintptr_t startAddress, uintptr_t endAddress, const std::string& stringPattern);
    private:
        std::vector<MEMORY_BASIC_INFORMATION> GetReadableMemoryRegions(uintptr_t startAddress, uintptr_t endAddress) const;
        MEMORY_BASIC_INFORMATION QueryVirtualMemory(uintptr_t address) const;
        bool IsMemoryRegionReadable(uintptr_t address) const;
        bool IsMemoryRegionUsed(uintptr_t address) const;

        PatternScanner patternScanner{};
    };
}
