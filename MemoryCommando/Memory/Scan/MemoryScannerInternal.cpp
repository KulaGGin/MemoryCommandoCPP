#include "MemoryScannerInternal.h"

namespace MemoryCommando::Memory {
    MemoryScannerInternal::MemoryScannerInternal(const std::shared_ptr<MemoryManagerInternal>& memoryManager) {
        _memoryManager = memoryManager;
    }

    std::vector<uintptr_t> MemoryScannerInternal::ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const BytePattern& bytePattern) {
        std::vector<uintptr_t> scanResults;

        auto memoryRegions = GetReadableMemoryRegions(desiredStartAddress, desiredEndAddress);
        for(auto& memoryRegion : memoryRegions) {
            uintptr_t scanStartAddress = (uintptr_t)memoryRegion.BaseAddress >= desiredStartAddress ? (uintptr_t)memoryRegion.BaseAddress : desiredStartAddress;
            uintptr_t regionEndAddress = (uintptr_t)memoryRegion.BaseAddress + memoryRegion.RegionSize - 1;
            uintptr_t scanEndAddress = regionEndAddress <= desiredEndAddress ? regionEndAddress : desiredEndAddress;
            auto memoryRegionScanResults = _patternScanner.Scan(scanStartAddress, scanEndAddress, bytePattern);
            if(!memoryRegionScanResults.empty())
                scanResults.insert(scanResults.end(), memoryRegionScanResults.begin(), memoryRegionScanResults.end());
        }

        return scanResults;
    }
}
