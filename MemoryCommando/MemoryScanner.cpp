#include "MemoryScanner.h"

#include "MemoryManager.h"
#include "VirtualQueryExException.h"


namespace MemoryCommando::External {
    MemoryScanner::MemoryScanner(MemoryManager& memoryManager) : _memoryManager(memoryManager) {

    }

    std::vector<MEMORY_BASIC_INFORMATION> MemoryScanner::GetMemoryRegions(uintptr_t startAddress, uintptr_t endAddress) {
        std::vector<MEMORY_BASIC_INFORMATION> memoryRegions;
        MEMORY_BASIC_INFORMATION memoryRegion;

        uintptr_t queryAddress = startAddress;

        while(queryAddress < endAddress) {
            // calculating new base address for a query right after current memory address
            memoryRegion = _memoryManager.QueryVirtualMemory(queryAddress);

            if(!(memoryRegion.Protect & _memoryFilter)) {
                memoryRegions.push_back(memoryRegion);
            }

            queryAddress = uintptr_t(memoryRegion.BaseAddress) + memoryRegion.RegionSize;
        }

        return memoryRegions;
    }

    std::vector<BYTE> MemoryScanner::GetMemoryRegionBytes(const _MEMORY_BASIC_INFORMATION& memoryRegion, uintptr_t scanStartAddress, uintptr_t scanEndAddress) {
        const uintptr_t memoryRegionBaseAddress = uintptr_t(memoryRegion.BaseAddress);
        const uintptr_t memoryRegionEndAddress = uintptr_t(memoryRegion.BaseAddress) + memoryRegion.RegionSize;
        uintptr_t readStartAddress = memoryRegionBaseAddress;
        uintptr_t readEndAddress = memoryRegionEndAddress;
        size_t readSize;

        if(scanStartAddress > memoryRegionBaseAddress) {
            readStartAddress = scanStartAddress;
        }
        if(scanEndAddress < memoryRegionEndAddress) {
            readEndAddress = scanEndAddress;
        }

        readSize = readEndAddress - readStartAddress;

        std::vector<BYTE> memoryRegionBytes = _memoryManager.ReadVirtualMemory(readStartAddress, readSize);

        return memoryRegionBytes;
    }

    std::vector<uintptr_t> MemoryScanner::Scan(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::string& pattern) {
        std::vector<uintptr_t> scanResults{};
        SYSTEM_INFO processInfo = _memoryManager.GetSystemInfo();
        const uintptr_t minimumApplicationAddress { uintptr_t(processInfo.lpMinimumApplicationAddress) };
        const uintptr_t maximumApplicationAddress { uintptr_t(processInfo.lpMaximumApplicationAddress) };

        if(scanStartAddress < minimumApplicationAddress)
            scanStartAddress = minimumApplicationAddress;
        if(scanEndAddress > maximumApplicationAddress)
            scanEndAddress = maximumApplicationAddress;

        std::vector<MEMORY_BASIC_INFORMATION> memoryRegions = GetMemoryRegions(scanStartAddress, scanEndAddress);

        for(auto memoryRegion : memoryRegions) {
            const std::vector<BYTE> memoryRegionBytes = GetMemoryRegionBytes(memoryRegion, scanStartAddress, scanEndAddress);

            std::vector<size_t> patternOffsets = _bytePatternScanner.Scan(memoryRegionBytes, pattern);

            for(auto patternOffset : patternOffsets) {
                uintptr_t patternAddress = uintptr_t(memoryRegion.BaseAddress) + patternOffset;
                scanResults.push_back(patternAddress);
            }
        }

        return scanResults;
    }
}
