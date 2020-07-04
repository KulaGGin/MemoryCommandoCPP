#pragma once
#include <CppUnitTest.h>

#include "../MemoryCommando/Memory/MemoryScanner.h"

namespace MemoryCommandoTests {
    using namespace MemoryCommando::Memory;
    using namespace External;
    TEST_CLASS(MemoryScannerTests) {
    public:
        MemoryScannerTests();

        TEST_METHOD(GetMemoryRegions);
        TEST_METHOD(Scan);
    private:
        MemoryManagerExternal _memoryManager = MemoryManagerExternal(GetCurrentProcessId());
        MemoryScanner _memoryScanner = MemoryScanner(_memoryManager);
        
    };
}
