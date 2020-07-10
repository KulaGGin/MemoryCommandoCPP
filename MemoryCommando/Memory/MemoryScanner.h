#pragma once

#include <vector>

#include "BytePatternScanner.h"
#include "External/MemoryManagerExternal.h"
#include <winnt.h>

#include "../HelperMethods.h"

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
        const DWORD _memoryFilter = PAGE_NOACCESS & PAGE_GUARD;
        const uintptr_t _minimumApplicationAddress;
        const uintptr_t _maximumApplicationAddress;

        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const;
        std::vector<MEMORY_BASIC_INFORMATION> GetMemoryRegions(uintptr_t startAddress, uintptr_t endAddress) const;

        friend class MemoryCommandoTests::MemoryScannerTests;
    };
}

