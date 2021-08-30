#pragma once
#include <activation.h>
#include <string>
#include <vector>

#include "HelperMethods.h"
#include "Memory/MemoryManager.h"
#include "Memory/Scan/BytePattern.h"
#include "Memory/Scan/PatternScanner.h"

namespace MemoryCommando::Memory {
    class MemoryScannerAbstract {
        FRIEND_TEST(MemoryScannerInternalF, IsMemoryRegionUsed);
        FRIEND_TEST(MemoryScannerInternalF, IsMemoryRegionReadable);
        FRIEND_TEST(MemoryScannerInternalF, GetsReadableMemoryRegions);
    public:
        MemoryScannerAbstract();
        virtual ~MemoryScannerAbstract() = default;

        virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const BytePattern& bytePattern) = 0;
        virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, const BytePattern& bytePattern);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const BytePattern& bytePattern);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const BytePattern& bytePattern);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const BytePattern& bytePattern);

        virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const std::string& stringPattern);
        virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, const std::string& stringPattern);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::string& stringPattern);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const std::string& stringPattern);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::string& stringPattern);

        virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, ByteSequence byteSequence);
        virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const ByteSequence& byteSequence);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const ByteSequence& byteSequence);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const ByteSequence& byteSequence);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const ByteSequence& byteSequence);

        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const Classname& object);
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const Classname& object);
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const Classname& object);
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, Classname object);
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, Classname object);

        std::vector<MEMORY_BASIC_INFORMATION> GetReadableMemoryRegions(uintptr_t startAddress, uintptr_t endAddress) const;
        bool IsMemoryRegionReadable(uintptr_t address) const;
        bool IsMemoryRegionUsed(uintptr_t address) const;
    protected:
        std::shared_ptr<MemoryManager> _memoryManager{};

        PatternScanner _patternScanner{};

        uintptr_t _minimumApplicationAddress{};
        uintptr_t _maximumApplicationAddress{};
    };

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const Classname& object) {
        ByteSequence objectBytesSequence = HelperMethods::ConvertObjectToByteSequence(object);
        auto scanResults = ScanVirtualMemory(scanStartAddress, scanEndAddress, objectBytesSequence);

        uintptr_t objectAddress = reinterpret_cast<uintptr_t>(&object);

        erase_if(scanResults, [objectAddress](uintptr_t currentScanResult) { return currentScanResult == objectAddress; });

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t scanStartAddress, const Classname& object) {
        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, _maximumApplicationAddress, object);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const Classname& object) {
        std::vector<uintptr_t> scanResults = ScanVirtualMemory(_minimumApplicationAddress, _maximumApplicationAddress, object);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::wstring& moduleName, Classname object) {
        MODULEENTRY32W moduleInst = _memoryManager->GetModule(moduleName);
        const uintptr_t scanStartAddress = uintptr_t(moduleInst.modBaseAddr);
        const uintptr_t scanEndAddress = uintptr_t(moduleInst.modBaseAddr + moduleInst.modBaseSize);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, scanEndAddress, object);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, Classname object) {
        std::vector<uintptr_t> scanResults;

        for(const auto& currentModule : moduleNames) {
            std::vector<uintptr_t> moduleResults = ScanVirtualMemory(currentModule, object);
            scanResults.insert(scanResults.end(), moduleResults.begin(), moduleResults.end());
        }

        return scanResults;
    }
}



