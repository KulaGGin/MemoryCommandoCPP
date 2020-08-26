#pragma once
#include <CppUnitTest.h>

#include "MemoryCommando.h"

namespace MemoryCommandoTests {
    TEST_CLASS(MemoryCommandoInternalTests) {
    public:
        MemoryCommando::MemoryCommando _memoryCommando{};
        MemoryCommandoInternalTests();

        TEST_METHOD(ReadVirtualMemory_FromArrayOfBytesAndOffset_ReturnArrayOfBytes);


    };
}

