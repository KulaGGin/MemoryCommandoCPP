#pragma once
#include <CppUnitTest.h>


#include "MemoryCommando/Memory/External/MemoryManagerExternal.h"
#include "MemoryCommando/Memory/Scan/MemoryScanner.h"
#include "MemoryCommandoTestsHelper.h"

namespace MemoryCommandoTests {
    using namespace Memory;
    using namespace External;
    TEST_CLASS(MemoryScannerTests) {
    public:
        MemoryScannerTests();

        TEST_METHOD(Constructor);
        TEST_METHOD(GetAccessibleMemoryRegions);
        TEST_METHOD(ScanStringStartEnd);
        TEST_METHOD(ScanStringStart);
        TEST_METHOD(ScanString);
        TEST_METHOD(ScanStringModuleName);
        TEST_METHOD(ScanStringModuleNames);

        TEST_METHOD(ScanBytePatternStartEnd);
        TEST_METHOD(ScanBytePatternStart);
        TEST_METHOD(ScanBytePattern);
        TEST_METHOD(ScanBytePatternModuleName);
        TEST_METHOD(ScanBytePatternModuleNames);

        TEST_METHOD(ScanObjectStartEnd);
        TEST_METHOD(ScanObjectStart);
        TEST_METHOD(ScanObject);
        TEST_METHOD(ScanObjectModuleName);
        TEST_METHOD(ScanObjectModuleNames);
    private:
        MemoryCommandoTestsHelper _memoryCommandoTestsHelper{};
        MemoryManagerExternal& _memoryManager;
        MemoryScanner _memoryScanner;
        const std::vector<BYTE> _testByteArray{ 0x43, 0x5C, 0xCA, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC, 0x62, 0xA0, 0x35, 0x34, 0x81 };
    };
}
