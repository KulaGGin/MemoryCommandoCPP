#pragma once
#include "gtest/gtest_prod.h"

#include "Memory/Scan/MemoryScannerAbstract.h"
#include "Memory/MemoryManager.h"
#include "Memory/Scan/PatternScanner.h"
#include "Exceptions/WinAPIException.h"

namespace MemoryCommando::Memory {
    using namespace Exceptions;
    class MemoryScannerInternal final : public MemoryScannerAbstract {
        FRIEND_TEST(MemoryScannerInternalF, IsMemoryRegionUsed);
        FRIEND_TEST(MemoryScannerInternalF, IsMemoryRegionReadable);
        FRIEND_TEST(MemoryScannerInternalF, GetsReadableMemoryRegions);
    public:
        MemoryScannerInternal(std::shared_ptr<MemoryManager> memoryManager)
            : _memoryManager{std::move(memoryManager)} {

        }

        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const std::string& stringPattern) override {
            ScanResults allResults;
            auto memoryRegions = GetReadableMemoryRegions(desiredStartAddress, desiredEndAddress);

            for(auto& memoryRegion : memoryRegions) {
                uintptr_t baseAddress = (uintptr_t)memoryRegion.BaseAddress;
                uintptr_t scanStartAddress = (uintptr_t)memoryRegion.BaseAddress >= desiredStartAddress ? (uintptr_t)memoryRegion.BaseAddress : desiredStartAddress;
                uintptr_t regionEndAddress = (uintptr_t)memoryRegion.BaseAddress + memoryRegion.RegionSize - 1;
                uintptr_t scanEndAddress = regionEndAddress <= desiredEndAddress ? regionEndAddress : desiredEndAddress;
                auto memoryRegionScanResults = _patternScanner.Scan(scanStartAddress, scanEndAddress, stringPattern);
                allResults.insert(allResults.end(), memoryRegionScanResults.begin(), memoryRegionScanResults.end());
            }

            return allResults;
        }
    private:
        std::vector<MEMORY_BASIC_INFORMATION> GetReadableMemoryRegions(uintptr_t startAddress, uintptr_t endAddress) const {
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

        bool IsMemoryRegionReadable(uintptr_t address) const {
            auto memoryInformation = _memoryManager->QueryVirtualMemory(address);

            bool isMemoryNoAccess = memoryInformation.Protect == PAGE_NOACCESS;

            bool isMemoryRegionReadable = !isMemoryNoAccess;

            return isMemoryRegionReadable;
        }

        bool IsMemoryRegionUsed(uintptr_t address) const {
            auto memoryInformation = _memoryManager->QueryVirtualMemory(address);

            bool isMemoryRegionUsed = memoryInformation.State == MEM_COMMIT;

            return isMemoryRegionUsed;
        }

        std::shared_ptr<MemoryManager> _memoryManager{};
        PatternScanner _patternScanner{};
    };
}

