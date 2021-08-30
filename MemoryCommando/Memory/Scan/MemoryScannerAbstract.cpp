#include "MemoryScannerAbstract.h"


#include "Exceptions/WinAPIException.h"
#include "Memory/Scan/PatternScanner.h"

namespace MemoryCommando::Memory {
    using namespace Exceptions;

    MemoryScannerAbstract::MemoryScannerAbstract() {
        _minimumApplicationAddress = uintptr_t(HelperMethods::GetSystemInfo().lpMinimumApplicationAddress);
        _maximumApplicationAddress = uintptr_t(HelperMethods::GetSystemInfo().lpMaximumApplicationAddress);
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t desiredStartAddress, const BytePattern& bytePattern) {
        auto scanResults = ScanVirtualMemory(desiredStartAddress, _maximumApplicationAddress, bytePattern);
        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const BytePattern& bytePattern) {
        auto scanResults = ScanVirtualMemory(_minimumApplicationAddress, _maximumApplicationAddress, bytePattern);
        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::wstring& moduleName, const BytePattern& bytePattern) {
        MODULEENTRY32W moduleInst = _memoryManager->GetModule(moduleName);
        const uintptr_t scanStartAddress = uintptr_t(moduleInst.modBaseAddr);
        const uintptr_t scanEndAddress = uintptr_t(moduleInst.modBaseAddr + moduleInst.modBaseSize);

        return ScanVirtualMemory(scanStartAddress, scanEndAddress, bytePattern);
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const BytePattern& bytePattern) {
        std::vector<uintptr_t> scanResults;

        for(const auto& currentModule : moduleNames) {
            std::vector<uintptr_t> moduleResults = ScanVirtualMemory(currentModule, bytePattern);
            scanResults.insert(scanResults.end(), moduleResults.begin(), moduleResults.end());
        }

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const std::string& stringPattern) {
        BytePattern bytePattern(stringPattern);

        ScanResults scanResults = ScanVirtualMemory(desiredStartAddress, desiredEndAddress, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t desiredStartAddress, const std::string& stringPattern) {
        BytePattern bytePattern{stringPattern};

        auto scanResults = ScanVirtualMemory(desiredStartAddress, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::string& stringPattern) {
        BytePattern bytePattern{stringPattern};

        auto scanResults = ScanVirtualMemory(bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::wstring& moduleName, const std::string& stringPattern) {
        BytePattern bytePattern{stringPattern};

        auto scanResults = ScanVirtualMemory(moduleName, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::string& stringPattern) {
        BytePattern bytePattern{stringPattern};

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(moduleNames, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, ByteSequence byteSequence) {
        BytePattern bytePattern{byteSequence};
        auto scanResults = ScanVirtualMemory(scanStartAddress, scanEndAddress, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t scanStartAddress, const ByteSequence& byteSequence) {
        BytePattern bytePattern{byteSequence};

        auto scanResults = ScanVirtualMemory(scanStartAddress, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const ByteSequence& byteSequence) {
        BytePattern bytePattern{byteSequence};

        auto scanResults = ScanVirtualMemory(bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::wstring& moduleName, const ByteSequence& byteSequence) {
        BytePattern bytePattern{byteSequence};

        auto scanResults = ScanVirtualMemory(moduleName, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const ByteSequence& byteSequence) {
        BytePattern bytePattern{byteSequence};

        auto scanResults = ScanVirtualMemory(moduleNames, bytePattern);

        return scanResults;
    }

    std::vector<MEMORY_BASIC_INFORMATION> MemoryScannerAbstract::GetReadableMemoryRegions(uintptr_t startAddress, uintptr_t endAddress) const {
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

    bool MemoryScannerAbstract::IsMemoryRegionReadable(uintptr_t address) const {
        auto memoryInformation = _memoryManager->QueryVirtualMemory(address);

        bool isMemoryNoAccess = memoryInformation.Protect & PAGE_NOACCESS;
        bool isMemoryGuard = memoryInformation.Protect & PAGE_GUARD;

        bool isMemoryRegionReadable = !isMemoryNoAccess && !isMemoryGuard;

        return isMemoryRegionReadable;
    }

    bool MemoryScannerAbstract::IsMemoryRegionUsed(uintptr_t address) const {
        auto memoryInformation = _memoryManager->QueryVirtualMemory(address);

        bool isMemoryRegionUsed = memoryInformation.State == MEM_COMMIT;

        return isMemoryRegionUsed;
    }
}


