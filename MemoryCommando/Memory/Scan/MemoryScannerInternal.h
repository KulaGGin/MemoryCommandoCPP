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
        MemoryScannerInternal(std::shared_ptr<MemoryManager> memoryManager);

        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const std::string& pattern) override;
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, const std::string& pattern) override;
        std::vector<uintptr_t> ScanVirtualMemory(const std::string& pattern) override;
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::string& pattern) override;
    private:
        std::vector<MEMORY_BASIC_INFORMATION> GetReadableMemoryRegions(uintptr_t startAddress, uintptr_t endAddress) const;
        bool IsMemoryRegionReadable(uintptr_t address) const;
        bool IsMemoryRegionUsed(uintptr_t address) const;

    public:
        std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const std::string& pattern) override;
    private:
        std::shared_ptr<MemoryManager> _memoryManager{};
        PatternScanner _patternScanner{};
    };
}