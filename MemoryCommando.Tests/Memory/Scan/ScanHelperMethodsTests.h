#pragma once
#include <CppUnitTest.h>

namespace MemoryCommandoTests {
    TEST_CLASS(ScanHelperMethodsTests) {
    public:
        TEST_METHOD(GetIndexedPatternString);
        TEST_METHOD(GetIndexedPatternStringBytes);
        TEST_METHOD(GetIndexedPatternBytes);
    };
}

