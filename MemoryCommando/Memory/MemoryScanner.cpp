#include "MemoryScanner.h"

#include "External/MemoryManagerExternal.h"
#include "ScanHelperMethods.h"

namespace MemoryCommando::Memory {
    MemoryScanner::MemoryScanner(const MemoryManager& memoryManager) :
    _memoryManager{ memoryManager },
    _minimumApplicationAddress{uintptr_t(HelperMethods::GetSystemInfo().lpMinimumApplicationAddress)},
    _maximumApplicationAddress{ uintptr_t(HelperMethods::GetSystemInfo().lpMaximumApplicationAddress) }{
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const {
        std::vector<uintptr_t> scanResults{};

        if(scanStartAddress < _minimumApplicationAddress)
            scanStartAddress = _minimumApplicationAddress;
        if(scanEndAddress > _maximumApplicationAddress)
            scanEndAddress = _maximumApplicationAddress;

        std::vector<MEMORY_BASIC_INFORMATION> memoryRegions = GetMemoryRegions(scanStartAddress, scanEndAddress);

        for(auto memoryRegion : memoryRegions) {
            const std::vector<BYTE> memoryRegionBytes = _memoryManager.ReadVirtualMemory(uintptr_t(memoryRegion.BaseAddress), size_t(memoryRegion.RegionSize));

            std::vector<size_t> patternOffsets = _bytePatternScanner.Scan(memoryRegionBytes, indexedPattern);

            for(auto patternOffset : patternOffsets) {
                uintptr_t patternAddress = uintptr_t(memoryRegion.BaseAddress) + patternOffset;
                if(patternAddress >= scanStartAddress && patternAddress <= scanEndAddress)
                    scanResults.push_back(patternAddress);
            }
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

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, _maximumApplicationAddress, indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::string& pattern) const {
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(pattern);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(_minimumApplicationAddress, _maximumApplicationAddress, indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::wstring& moduleName, const std::string& pattern) const {
        MODULEENTRY32W module = _memoryManager.GetModule(moduleName);
        const uintptr_t scanStartAddress = uintptr_t(module.modBaseAddr);
        const uintptr_t scanEndAddress = uintptr_t(module.modBaseAddr + module.modBaseSize);

        return ScanVirtualMemory(scanStartAddress, scanEndAddress, pattern);
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::string& pattern) const {
        std::vector<uintptr_t> scanResults;

        for(const auto& module : moduleNames) {
            std::vector<uintptr_t> moduleResults = ScanVirtualMemory(module, pattern);
            scanResults.insert(scanResults.end(), moduleResults.begin(), moduleResults.end());
        }

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const uintptr_t scanStartAddress, const uintptr_t scanEndAddress, const std::vector<BYTE>& bytePattern) const {
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(bytePattern);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, scanEndAddress, indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const uintptr_t scanStartAddress, const std::vector<BYTE>& bytePattern) const {
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(bytePattern);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, _maximumApplicationAddress, indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::vector<BYTE>& bytePattern) const {
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(bytePattern);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(_minimumApplicationAddress, _maximumApplicationAddress, indexedPattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::wstring& moduleName, const std::vector<BYTE>& bytePattern) const {
        MODULEENTRY32W module = _memoryManager.GetModule(moduleName);
        const uintptr_t scanStartAddress = uintptr_t(module.modBaseAddr);
        const uintptr_t scanEndAddress = uintptr_t(module.modBaseAddr + module.modBaseSize);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, scanEndAddress, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::vector<BYTE>& bytePattern) const {
        std::vector<uintptr_t> scanResults;

        for(const auto& module : moduleNames) {
            std::vector<uintptr_t> moduleResults = ScanVirtualMemory(module, bytePattern);
            scanResults.insert(scanResults.end(), moduleResults.begin(), moduleResults.end());
        }

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const Classname& object) const {
        auto objectBytesSequence = HelperMethods::ConvertObjectToBytes(object);
        auto scanResults = ScanVirtualMemory(scanStartAddress, scanEndAddress, objectBytesSequence);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(uintptr_t scanStartAddress, const Classname& object) const {
        auto objectByteSequence = HelperMethods::ConvertObjectToBytes(object);

        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(objectByteSequence);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, _maximumApplicationAddress, indexedPattern);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const Classname& object) const {
        auto objectByteSequence = HelperMethods::ConvertObjectToBytes(object);

        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(objectByteSequence);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(_minimumApplicationAddress, _maximumApplicationAddress, indexedPattern);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::wstring& moduleName, Classname object) const {
        MODULEENTRY32W module = _memoryManager.GetModule(moduleName);
        const uintptr_t scanStartAddress = uintptr_t(module.modBaseAddr);
        const uintptr_t scanEndAddress = uintptr_t(module.modBaseAddr + module.modBaseSize);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, scanEndAddress, object);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, Classname object) const {
        std::vector<uintptr_t> scanResults;

        for(const auto& module : moduleNames) {
            std::vector<uintptr_t> moduleResults = ScanVirtualMemory(module, object);
            scanResults.insert(scanResults.end(), moduleResults.begin(), moduleResults.end());
        }

        return scanResults;
    }

    std::vector<MEMORY_BASIC_INFORMATION> MemoryScanner::GetMemoryRegions(const uintptr_t startAddress, const uintptr_t endAddress) const {
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

}
