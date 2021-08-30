#pragma once
#include "gtest/gtest_prod.h"

#include "Memory/Scan/MemoryScannerAbstract.h"
#include "Memory/MemoryManager.h"
#include "Memory/Scan/PatternScanner.h"
#include "Exceptions/WinAPIException.h"

namespace MemoryCommando::Memory {
    
    class MemoryScannerInternal final : public MemoryScannerAbstract {
    public:
        MemoryScannerInternal(const std::shared_ptr<MemoryManager>& memoryManager);
        using MemoryScannerAbstract::ScanVirtualMemory;

        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const BytePattern& bytePattern) override;
    };
}