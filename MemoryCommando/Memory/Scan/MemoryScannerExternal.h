#pragma once
#include "Memory/MemoryManager.h"
#include "Memory/Internal/MemoryManagerInternal.h"
#include "Memory/Scan/MemoryScanner.h"
#include "Memory/Scan/MemoryScannerInternal.h"

namespace MemoryCommando::Memory {
    using namespace Internal;
    class MemoryScannerExternal : public MemoryScanner {
    public:
        MemoryScannerExternal() = default;
        MemoryScannerExternal(std::shared_ptr<MemoryManager> memoryManager);

        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const BytePattern& bytePattern) override;
    private:
        std::shared_ptr<MemoryManagerInternal> _memoryManagerInternal{};
        std::shared_ptr<MemoryScannerInternal> _memoryScannerInternal{};
    };
}

