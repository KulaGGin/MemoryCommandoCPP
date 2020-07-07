#include "MemoryScanner.h"

#include "External/MemoryManagerExternal.h"
#include "../Exceptions/VirtualQueryExException.h"


namespace MemoryCommando::Memory {
    MemoryScanner::MemoryScanner(MemoryManager& memoryManager) : _memoryManager(memoryManager) {

    }

    std::vector<MEMORY_BASIC_INFORMATION> MemoryScanner::GetMemoryRegions(const uintptr_t startAddress, const uintptr_t endAddress) {
        std::vector<MEMORY_BASIC_INFORMATION> memoryRegions;

        uintptr_t queryAddress = startAddress;

        while(queryAddress < endAddress) {
            // calculating new base address for a query right after current memory address
            MEMORY_BASIC_INFORMATION memoryRegion = _memoryManager.QueryVirtualMemory(queryAddress);

            if(!(memoryRegion.Protect & _memoryFilter)) {
                memoryRegions.push_back(memoryRegion);
            }

            queryAddress = uintptr_t(memoryRegion.BaseAddress) + memoryRegion.RegionSize;
        }

        return memoryRegions;
    }

    std::vector<uintptr_t> MemoryScanner::Scan(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::string& pattern) {
        std::vector<uintptr_t> scanResults{};
        SYSTEM_INFO processInfo = MemoryManager::GetSystemInfo();
        const uintptr_t minimumApplicationAddress { uintptr_t(processInfo.lpMinimumApplicationAddress) };
        const uintptr_t maximumApplicationAddress { uintptr_t(processInfo.lpMaximumApplicationAddress) };

        if(scanStartAddress < minimumApplicationAddress)
            scanStartAddress = minimumApplicationAddress;
        if(scanEndAddress > maximumApplicationAddress)
            scanEndAddress = maximumApplicationAddress;

        std::vector<MEMORY_BASIC_INFORMATION> memoryRegions = GetMemoryRegions(scanStartAddress, scanEndAddress);

        for(auto memoryRegion : memoryRegions) {
            const std::vector<BYTE> memoryRegionBytes = _memoryManager.ReadVirtualMemory(uintptr_t(memoryRegion.BaseAddress), size_t(memoryRegion.RegionSize));

            std::vector<size_t> patternOffsets = _bytePatternScanner.Scan(memoryRegionBytes, pattern);

            for(auto patternOffset : patternOffsets) {
                uintptr_t patternAddress = uintptr_t(memoryRegion.BaseAddress) + patternOffset;
                if(patternAddress >= scanStartAddress && patternAddress <= scanEndAddress)
                    scanResults.push_back(patternAddress);
            }
        }

        return scanResults;
    }
}
