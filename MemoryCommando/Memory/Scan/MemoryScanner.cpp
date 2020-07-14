#include "MemoryScanner.h"

#include <algorithm>
#include <utility>

#include "Exceptions/WinAPIException.h"
#include "Memory/External/MemoryManagerExternal.h"
#include "Native/Enumerations/MemoryProtection.h"
#include "ScanHelperMethods.h"

namespace MemoryCommando::Memory {
    MemoryScanner::MemoryScanner(std::shared_ptr<const MemoryManager> memoryManager) :
        _memoryManager{ std::move(memoryManager) },
    _minimumApplicationAddress{uintptr_t(HelperMethods::GetSystemInfo().lpMinimumApplicationAddress)},
    _maximumApplicationAddress{ uintptr_t(HelperMethods::GetSystemInfo().lpMaximumApplicationAddress) }{
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const {
        std::vector<uintptr_t> scanResults{};

        if(scanStartAddress < _minimumApplicationAddress)
            scanStartAddress = _minimumApplicationAddress;
        if(scanEndAddress > _maximumApplicationAddress)
            scanEndAddress = _maximumApplicationAddress;

        std::vector<MEMORY_BASIC_INFORMATION> memoryRegions = GetAccessibleUsedMemoryRegions(scanStartAddress, scanEndAddress);

        for(auto memoryRegion : memoryRegions) {
            const std::vector<BYTE> memoryRegionBytes = _memoryManager->ReadVirtualMemory(uintptr_t(memoryRegion.BaseAddress), size_t(memoryRegion.RegionSize));

            std::vector<size_t> patternOffsets = _bytePatternScanner.Scan(memoryRegionBytes, indexedPattern);

            for(auto patternOffset : patternOffsets) {
                uintptr_t patternAddress = uintptr_t(memoryRegion.BaseAddress) + patternOffset;
                if(patternAddress >= scanStartAddress && patternAddress <= scanEndAddress)
                    scanResults.push_back(patternAddress);
            }
        }

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(uintptr_t scanStartAddress, const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const {
        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, _maximumApplicationAddress, indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const {
        std::vector<uintptr_t> scanResults = ScanVirtualMemory(_minimumApplicationAddress, _maximumApplicationAddress, indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::wstring& moduleName, const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const {
        MODULEENTRY32W module = _memoryManager->GetModule(moduleName);
        const uintptr_t scanStartAddress = uintptr_t(module.modBaseAddr);
        const uintptr_t scanEndAddress = uintptr_t(module.modBaseAddr + module.modBaseSize);

        return ScanVirtualMemory(scanStartAddress, scanEndAddress, indexedPattern);
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const {
        std::vector<uintptr_t> scanResults;

        for(const auto& module : moduleNames) {
            std::vector<uintptr_t> moduleResults = ScanVirtualMemory(module, indexedPattern);
            scanResults.insert(scanResults.end(), moduleResults.begin(), moduleResults.end());
        }

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const uintptr_t scanStartAddress, const uintptr_t scanEndAddress, const std::string& pattern) const {
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(pattern);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, scanEndAddress, indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const uintptr_t scanStartAddress, const std::string& pattern) const {
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(pattern);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::string& pattern) const {
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(pattern);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::wstring& moduleName, const std::string& pattern) const {
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(pattern);
        
        std::vector<uintptr_t> scanResults = ScanVirtualMemory(moduleName, indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::string& pattern) const {
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(pattern);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(moduleNames, indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const uintptr_t scanStartAddress, const uintptr_t scanEndAddress, const std::vector<BYTE>& bytePattern) const {
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(bytePattern);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, scanEndAddress, indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const uintptr_t scanStartAddress, const std::vector<BYTE>& bytePattern) const {
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(bytePattern);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::vector<BYTE>& bytePattern) const {
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(bytePattern);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::wstring& moduleName, const std::vector<BYTE>& bytePattern) const {
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(bytePattern);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(moduleName, indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::vector<BYTE>& bytePattern) const {
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(bytePattern);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(moduleNames, indexedPattern);

        return scanResults;
    }

    std::vector<MEMORY_BASIC_INFORMATION> MemoryScanner::GetAccessibleUsedMemoryRegions(const uintptr_t startAddress, const uintptr_t endAddress) const {
        std::vector<MEMORY_BASIC_INFORMATION> memoryRegions{};

        uintptr_t queryAddress = startAddress;
        MEMORY_BASIC_INFORMATION memoryRegion;

        while(queryAddress < endAddress) {
            // calculating new base address for a query right after current memory address
            try {
                memoryRegion = _memoryManager->QueryVirtualMemory(queryAddress);
            }
            catch(const Exceptions::WinAPIException& exception) {
                queryAddress += 0x1000;
                continue;
            }

            if(CanAccessMemoryRegion(memoryRegion) && IsMemoryRegionUsed(memoryRegion)) {
                memoryRegions.push_back(memoryRegion);
            }

            queryAddress = uintptr_t(memoryRegion.BaseAddress) + memoryRegion.RegionSize;
        }

        return memoryRegions;
    }

    bool MemoryScanner::CanAccessMemoryRegion(const MEMORY_BASIC_INFORMATION memoryRegion) const {
        const bool canAccess = std::all_of(_memoryFilterList.begin(), _memoryFilterList.end(),
                                           [memoryRegion](MemoryProtection memoryProtection) { return !(memoryRegion.Protect == DWORD(memoryProtection) || memoryRegion.Protect & DWORD(memoryProtection)); });

        return canAccess;
    }

    bool MemoryScanner::IsMemoryRegionUsed(const MEMORY_BASIC_INFORMATION memoryRegion) const {
        const bool memoryRegionUsed = memoryRegion.State == MEM_COMMIT;

        return memoryRegionUsed;
    }
}
