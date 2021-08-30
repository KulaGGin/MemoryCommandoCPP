#include "gtest/gtest.h"
#include "Memory/External/MemoryManagerExternal.h"
#include "Memory/Internal/MemoryManagerInternal.h"
#include "Memory/Scan/MemoryScannerAbstract.h"
#include "Memory/Scan/MemoryScannerInternal.h"
#include "Memory/Scan/MemoryScannerExternal.h"

namespace MemoryCommando::Memory {
    using namespace External;
    TEST(MemoryScannerExternal, GivenStartAndEndAddressAndBytePattern_ScansCorrectly) {
        std::shared_ptr<MemoryManager> memoryManagerExternal = std::make_shared<MemoryManagerExternal>(GetCurrentProcessId());
        MemoryScannerExternal memoryScanner = {memoryManagerExternal};
        
        std::vector<BYTE> byteText = {0x43, 0x5C, 0xCA, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC};
        BytePattern bytePattern("CA F0 18 33 17 F0 31 BC C4 FC");

        uintptr_t scanStartAddress = (uintptr_t)&byteText.front();
        uintptr_t scanEndAddress = (uintptr_t)&byteText.back();
        auto scanResults = memoryScanner.ScanVirtualMemory(scanStartAddress, scanEndAddress, bytePattern);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults[0], (uintptr_t)&byteText[2]);
    }
}

