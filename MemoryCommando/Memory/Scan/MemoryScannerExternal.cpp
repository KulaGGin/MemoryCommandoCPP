#include "MemoryScannerExternal.h"

namespace MemoryCommando::Memory {

    MemoryScannerExternal::MemoryScannerExternal(std::shared_ptr<MemoryManager> memoryManager) {
        _memoryManager = std::move(memoryManager);
        _memoryManagerInternal = std::make_shared<MemoryManagerInternal>();
        _memoryScannerInternal = std::make_shared<MemoryScannerInternal>(_memoryManagerInternal);
    }

    std::vector<uintptr_t> MemoryScannerExternal::ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const BytePattern& bytePattern) {
        ScanResults scanResults;

        auto memoryRegions = GetReadableMemoryRegions(desiredStartAddress, desiredEndAddress);
        for(auto& memoryRegion : memoryRegions) {
            const std::vector<BYTE> currentMemoryRegionBytes = _memoryManager->ReadVirtualMemory((uintptr_t)memoryRegion.BaseAddress, (size_t)memoryRegion.RegionSize - 1);

            auto copiedMemoryBytesScanResults = _memoryScannerInternal->ScanVirtualMemory((uintptr_t)&currentMemoryRegionBytes.front(), (uintptr_t)&currentMemoryRegionBytes.back(), bytePattern);
            for(const auto& currentScanResult : copiedMemoryBytesScanResults) {
                uintptr_t addressInScannedProcess = currentScanResult - (uintptr_t)&currentMemoryRegionBytes.front() + (uintptr_t)memoryRegion.BaseAddress;
                scanResults.push_back(addressInScannedProcess);
            }
        }

        return scanResults;
    }
}
