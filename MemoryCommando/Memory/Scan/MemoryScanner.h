#pragma once

#include <vector>

#include "BytePatternScanner.h"
#include "Memory/External/MemoryManagerExternal.h"
#include <winnt.h>

#include "HelperMethods.h"
#include "Native/Enumerations/MemoryProtection.h"

namespace MemoryCommandoTests {
    class MemoryScannerTests;
}

namespace MemoryCommando::Memory {
    class MemoryScanner {
    public:
        MemoryScanner(const MemoryManager &memoryManager);
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::string& pattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const std::string& pattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::string& pattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const std::string& pattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::string& pattern) const;

        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::vector<BYTE>& bytePattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const std::vector<BYTE>& bytePattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<BYTE>& bytePattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const std::vector<BYTE>& bytePattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::vector<BYTE>& bytePattern) const;

        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const Classname& object) const;
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const Classname& object) const;
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const Classname& object) const;
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, Classname object) const;
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, Classname object) const;

    private:
        const MemoryManager& _memoryManager;
        const BytePatternScanner _bytePatternScanner{};
        const DWORD _memoryFilter = PAGE_NOACCESS | PAGE_GUARD;
        const std::vector<MemoryProtection> _memoryFilterList = { MemoryProtection::ZeroAccess, MemoryProtection::NoAccess, MemoryProtection::Guard };
        const uintptr_t _minimumApplicationAddress;
        const uintptr_t _maximumApplicationAddress;

        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const;
        std::vector<MEMORY_BASIC_INFORMATION> GetAccessibleUsedMemoryRegions(uintptr_t startAddress, uintptr_t endAddress) const;
        bool CanAccessMemoryRegion(MEMORY_BASIC_INFORMATION memoryRegion) const;
        bool IsMemoryRegionUsed(MEMORY_BASIC_INFORMATION memoryRegion) const;

        friend class MemoryCommandoTests::MemoryScannerTests;
    };


    template <typename Classname>
    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const Classname& object) const {
        auto objectBytesSequence = HelperMethods::ConvertObjectToBytes(object);
        auto scanResults = ScanVirtualMemory(scanStartAddress, scanEndAddress, objectBytesSequence);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(uintptr_t scanStartAddress, const Classname& object) const {
        auto objectByteSequence = HelperMethods::ConvertObjectToBytes(object);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, objectByteSequence);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const Classname& object) const {
        auto objectByteSequence = HelperMethods::ConvertObjectToBytes(object);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(_minimumApplicationAddress, objectByteSequence);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::wstring& moduleName, Classname object) const {
        auto objectByteSequence = HelperMethods::ConvertObjectToBytes(object);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(moduleName, objectByteSequence);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScanner::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, Classname object) const {
        auto objectByteSequence = HelperMethods::ConvertObjectToBytes(object);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(moduleNames, objectByteSequence);

        return scanResults;
    }
}