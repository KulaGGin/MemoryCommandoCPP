#include "MemoryScannerInternal.h"

namespace MemoryCommando::Memory {
    MemoryScannerInternal::MemoryScannerInternal(std::shared_ptr<MemoryManager> memoryManager)
    : _memoryManager{std::move(memoryManager)} {

    }

    std::vector<uintptr_t> MemoryScannerInternal::ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const std::string& stringPattern) {
        ScanResults allResults;
        auto memoryRegions = GetReadableMemoryRegions(desiredStartAddress, desiredEndAddress);

        for(auto& memoryRegion : memoryRegions) {
            uintptr_t scanStartAddress = (uintptr_t)memoryRegion.BaseAddress >= desiredStartAddress ? (uintptr_t)memoryRegion.BaseAddress : desiredStartAddress;
            uintptr_t regionEndAddress = (uintptr_t)memoryRegion.BaseAddress + memoryRegion.RegionSize - 1;
            uintptr_t scanEndAddress = regionEndAddress <= desiredEndAddress ? regionEndAddress : desiredEndAddress;
            auto memoryRegionScanResults = _patternScanner.Scan(scanStartAddress, scanEndAddress, stringPattern);
            allResults.insert(allResults.end(), memoryRegionScanResults.begin(), memoryRegionScanResults.end());
        }

        return allResults;
    }

    std::vector<MEMORY_BASIC_INFORMATION> MemoryScannerInternal::GetReadableMemoryRegions(uintptr_t startAddress, uintptr_t endAddress) const {
        std::vector<MEMORY_BASIC_INFORMATION> memoryRegions{};

        uintptr_t queryAddress = startAddress;
        MEMORY_BASIC_INFORMATION memoryRegion;

        while(queryAddress < endAddress) {
            try {
                memoryRegion = _memoryManager->QueryVirtualMemory(queryAddress);
            }
            catch(const WinAPIException&) {
                queryAddress += 0x1000;
                continue;
            }
            if(IsMemoryRegionUsed(queryAddress) && IsMemoryRegionReadable(queryAddress))
                memoryRegions.push_back(memoryRegion);

            queryAddress = uintptr_t(memoryRegion.BaseAddress) + memoryRegion.RegionSize;
        }

        return memoryRegions;
    }

    bool MemoryScannerInternal::IsMemoryRegionReadable(uintptr_t address) const {
        auto memoryInformation = _memoryManager->QueryVirtualMemory(address);

        bool isMemoryNoAccess = memoryInformation.Protect == PAGE_NOACCESS;

        bool isMemoryRegionReadable = !isMemoryNoAccess;

        return isMemoryRegionReadable;
    }

    bool MemoryScannerInternal::IsMemoryRegionUsed(uintptr_t address) const {
        auto memoryInformation = _memoryManager->QueryVirtualMemory(address);

        bool isMemoryRegionUsed = memoryInformation.State == MEM_COMMIT;

        return isMemoryRegionUsed;
    }

}
