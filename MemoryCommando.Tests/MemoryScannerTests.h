#pragma once
#include <CppUnitTest.h>

#include "../MemoryCommando/Memory/External/MemoryScanner.h"

namespace MemoryCommandoTests {
    using namespace MemoryCommando::External;
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
