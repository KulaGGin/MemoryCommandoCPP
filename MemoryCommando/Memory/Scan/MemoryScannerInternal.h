#pragma once
#include "gtest/gtest_prod.h"

#include "Memory/Scan/MemoryScannerAbstract.h"
#include "Memory/MemoryManager.h"
#include "Memory/Scan/PatternScanner.h"
#include "Exceptions/WinAPIException.h"

namespace MemoryCommando::Memory {
    
    class MemoryScannerInternal final : public MemoryScannerAbstract {
        FRIEND_TEST(MemoryScannerInternalF, GivenStartAndEndAddressAndBytePattern_ScansCorrectly);
        FRIEND_TEST(MemoryScannerInternalF, GivenStartAddressAndBytePattern_ScansCorrectly);
        FRIEND_TEST(MemoryScannerInternalF, GivenOnlyBytePattern_ScansCorrectly);
        FRIEND_TEST(MemoryScannerInternalF, GivenModuleNameAndBytePattern_ScansCorrectly);
        FRIEND_TEST(MemoryScannerInternalF, GivenSequenceOfModuleNamesAndBytePattern_ScansCorrectly);
    public:
        MemoryScannerInternal(const std::shared_ptr<MemoryManager>& memoryManager);
        using MemoryScannerAbstract::ScanVirtualMemory;
    protected:
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const BytePattern& bytePattern) override;
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, const BytePattern& bytePattern) override;
        std::vector<uintptr_t> ScanVirtualMemory(const BytePattern& bytePattern) override;
        std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const BytePattern& bytePattern) override;
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const BytePattern& bytePattern) override;
    };
}