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

        FRIEND_TEST(MemoryScannerInternalF, GivenStartAndEndAddressAndBytePattern_ScansCorrectly);
        FRIEND_TEST(MemoryScannerInternalF, GivenStartAddressAndBytePattern_ScansCorrectly);
        FRIEND_TEST(MemoryScannerInternalF, GivenOnlyBytePattern_ScansCorrectly);
        FRIEND_TEST(MemoryScannerInternalF, GivenModuleNameAndBytePattern_ScansCorrectly);
        FRIEND_TEST(MemoryScannerInternalF, GivenSequenceOfModuleNamesAndBytePattern_ScansCorrectly);
    public:
        using MemoryScannerAbstract::ScanVirtualMemory;
        MemoryScannerInternal(std::shared_ptr<MemoryManager> memoryManager);
    protected:
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const BytePattern& bytePattern) override;
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, const BytePattern& bytePattern) override;
        std::vector<uintptr_t> ScanVirtualMemory(const BytePattern& bytePattern) override;
        std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const BytePattern& bytePattern) override;
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const BytePattern& bytePattern) override;
    private:
        std::vector<MEMORY_BASIC_INFORMATION> GetReadableMemoryRegions(uintptr_t startAddress, uintptr_t endAddress) const;
        bool IsMemoryRegionReadable(uintptr_t address) const;
        bool IsMemoryRegionUsed(uintptr_t address) const;
        
        std::shared_ptr<MemoryManager> _memoryManager{};
        PatternScanner _patternScanner{};
    };
}