#include "MemoryScannerInternal.h"

namespace MemoryCommando::Memory {
    MemoryScannerInternal::MemoryScannerInternal(const std::shared_ptr<MemoryManager>& memoryManager) {
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

    std::vector<uintptr_t> MemoryScannerInternal::ScanVirtualMemory(uintptr_t desiredStartAddress, const BytePattern& bytePattern) {
        auto scanResults = ScanVirtualMemory(desiredStartAddress, _maximumApplicationAddress, bytePattern);
        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerInternal::ScanVirtualMemory(const BytePattern& bytePattern) {
        auto scanResults = ScanVirtualMemory(_minimumApplicationAddress, _maximumApplicationAddress, bytePattern);
        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerInternal::ScanVirtualMemory(const std::wstring& moduleName, const BytePattern& bytePattern) {
        MODULEENTRY32W moduleInst = _memoryManager->GetModule(moduleName);
        const uintptr_t scanStartAddress = uintptr_t(moduleInst.modBaseAddr);
        const uintptr_t scanEndAddress = uintptr_t(moduleInst.modBaseAddr + moduleInst.modBaseSize);

        return ScanVirtualMemory(scanStartAddress, scanEndAddress, bytePattern);
    }

    std::vector<uintptr_t> MemoryScannerInternal::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const BytePattern& bytePattern) {
        std::vector<uintptr_t> scanResults;

        for(const auto& currentModule : moduleNames) {
            std::vector<uintptr_t> moduleResults = ScanVirtualMemory(currentModule, bytePattern);
            scanResults.insert(scanResults.end(), moduleResults.begin(), moduleResults.end());
        }

        return scanResults;
    }
}
