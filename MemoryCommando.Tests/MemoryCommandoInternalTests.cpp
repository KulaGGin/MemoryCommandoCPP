#include "MemoryCommandoInternalTests.h"

#include "MemoryCommando.h"

namespace MemoryCommandoTests {
    MemoryCommandoInternalTests::MemoryCommandoInternalTests() {
        
    }

    void MemoryCommandoInternalTests::ReadVirtualMemory_FromArrayOfBytesAndOffset_ReturnArrayOfBytes() {
        std::vector<BYTE> bytes{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        

        _memoryCommando.ReadVirtualMemory(std::vector<BYTE>{0, 1, 2, 3, 4, 5, 6, 7, 8}, 8, 1);
    }
}
