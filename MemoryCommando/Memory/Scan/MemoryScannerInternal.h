#pragma once
#include "gtest/gtest_prod.h"

#include "Memory/Scan/MemoryScanner.h"
#include "Memory/MemoryManager.h"
#include "Memory/Scan/PatternScanner.h"
#include "Exceptions/WinAPIException.h"
#include "Memory/Internal/MemoryManagerInternal.h"

namespace MemoryCommando::Memory {
    using namespace Internal;
    class MemoryScannerInternal final : public MemoryScanner {
    public:
        MemoryScannerInternal(const std::shared_ptr<MemoryManagerInternal>& memoryManager);
        using MemoryScanner::ScanVirtualMemory;

        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const BytePattern& bytePattern) override;
    };
}
