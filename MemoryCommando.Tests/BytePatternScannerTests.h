#pragma once

#include <CppUnitTest.h>

#include "../MemoryCommando/BytePatternScanner.h"

namespace MemoryCommandoTests {
    TEST_CLASS(BytePatternScannerTests) {
    public:
        BytePatternScannerTests();

        TEST_METHOD(GetLastWildcardIndex);
        TEST_METHOD(GetFilteredPattern);
        TEST_METHOD(GenerateBadByteTable);
        TEST_METHOD(Scan);
    private:
        MemoryCommando::Scan::BytePatternScanner _bytePatternScanner;
    };
};