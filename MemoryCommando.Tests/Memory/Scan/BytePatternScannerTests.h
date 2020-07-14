#pragma once

#include <CppUnitTest.h>

#include "MemoryCommando/Memory/Scan/BytePatternScanner.h"

namespace MemoryCommandoTests {
    using namespace MemoryCommando;
    using namespace Memory;
    TEST_CLASS(BytePatternScannerTests) {
    public:
        BytePatternScannerTests();

        TEST_METHOD(GenerateBadByteTable);
        TEST_METHOD(ScanIndexedBytePattern);
        TEST_METHOD(ScanBytePattern);
        TEST_METHOD(ScanStringPattern);

    private:
        BytePatternScanner _bytePatternScanner;
    };
};
