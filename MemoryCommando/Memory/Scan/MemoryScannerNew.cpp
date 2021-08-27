#include "MemoryScannerNew.h"

#include "Exceptions/VirtualQueryException.h"

namespace MemoryCommando::Memory {
    using namespace Exceptions;

    ScanResults MemoryScannerNew::Scan(uintptr_t startAddress, uintptr_t endAddress, const std::string& stringPattern) {
        ScanResults allResults;
        auto memoryRegions = GetReadableMemoryRegions(startAddress, endAddress);

        for(auto& memoryRegion : memoryRegions) {
            uintptr_t baseAddress = (uintptr_t)memoryRegion.BaseAddress;
            uintptr_t scanStartAddress = (uintptr_t)memoryRegion.BaseAddress >= startAddress ? (uintptr_t)memoryRegion.BaseAddress : startAddress;
            uintptr_t regionEndAddress = (uintptr_t)memoryRegion.BaseAddress + memoryRegion.RegionSize - 1;
            uintptr_t scanEndAddress = regionEndAddress <= endAddress ? regionEndAddress : endAddress;
            auto memoryRegionScanResults = patternScanner.Scan(scanStartAddress, scanEndAddress, stringPattern);
            allResults.insert(allResults.end(), memoryRegionScanResults.begin(), memoryRegionScanResults.end());
        }

        return allResults;
    }

    std::vector<MEMORY_BASIC_INFORMATION> MemoryScannerNew::GetReadableMemoryRegions(uintptr_t startAddress, uintptr_t endAddress) const {
        std::vector<MEMORY_BASIC_INFORMATION> memoryRegions{};

        uintptr_t queryAddress = startAddress;
        MEMORY_BASIC_INFORMATION memoryRegion;

        while(queryAddress < endAddress) {
            try {
                memoryRegion = QueryVirtualMemory(queryAddress);
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

    MEMORY_BASIC_INFORMATION MemoryScannerNew::QueryVirtualMemory(uintptr_t address) const {
        MEMORY_BASIC_INFORMATION memoryBasicInformation{};

        const SIZE_T bytesReturned = VirtualQuery(reinterpret_cast<LPVOID>(address), &memoryBasicInformation, sizeof memoryBasicInformation);

        if(!bytesReturned) {
            std::stringstream errorMessage;
            errorMessage << "VirtualQuery couldn't query memory and failed with error code " << GetLastError();
            throw VirtualQueryException(errorMessage.str(), GetLastError(), address);
        }

        return memoryBasicInformation;
    }

    bool MemoryScannerNew::IsMemoryRegionReadable(uintptr_t address) const {
        auto memoryInformation = QueryVirtualMemory(address);
        bool isMemoryNoAccess = memoryInformation.Protect == PAGE_NOACCESS;

        bool isMemoryRegionReadable = !isMemoryNoAccess;

        return isMemoryRegionReadable;
    }

    bool MemoryScannerNew::IsMemoryRegionUsed(uintptr_t address) const {
        auto memoryInformation = QueryVirtualMemory(address);
        bool isMemoryRegionUsed = memoryInformation.State == MEM_COMMIT;

        return isMemoryRegionUsed;
    }
}