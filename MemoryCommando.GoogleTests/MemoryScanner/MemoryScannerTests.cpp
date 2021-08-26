#include "gtest/gtest.h"
#include "Memory/Scan/PatternScanner/MemoryScannerNew.h"

namespace MemoryCommando::Memory {
    TEST(MemoryScanner, ScansContiguousMemory) {
        MemoryScannerNew memoryScanner{};
        std::vector<BYTE> byteText{0x00, 0x01, 0x05, 0x10, 0xFC};
        ScanResults scanResults = memoryScanner.Scan((uintptr_t)&byteText.front(), (uintptr_t)&byteText.back(), "05 10 FC");

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults[0], (uintptr_t)&byteText[2]);
    }
    TEST(MemoryScanner, ScansTwoMemoryRegionsWithUnreachableMemoryBetweenThem) {
        MemoryScannerNew memoryScanner{};
        std::vector<BYTE> byteText{0x00, 0x01, 0x05, 0x10, 0xFC};
        size_t allocationSize = 0x3000;
        uintptr_t allocationAddress = reinterpret_cast<uintptr_t>(VirtualAlloc(nullptr, allocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
        uintptr_t endAddress = allocationAddress + allocationSize;
        if(!allocationAddress)
            FAIL("Couldn't allocate memory with VirtualAlloc");

        DWORD flOldProtect;
        LPVOID protectAddress = reinterpret_cast<LPVOID>(allocationAddress + 0x1000);
        VirtualProtect(protectAddress, 0x1000, PAGE_NOACCESS, &flOldProtect);

        uintptr_t thirdMemoryRegionAddress = allocationAddress + 0x2000;

        std::memcpy((LPVOID)allocationAddress, &byteText[0], byteText.size());
        std::memcpy((LPVOID)thirdMemoryRegionAddress, &byteText[0], byteText.size());

        auto results = memoryScanner.Scan(allocationAddress, endAddress, "05 10 FC");
        ASSERT_EQ(results.size(), 2);
        ASSERT_EQ(results[0], allocationAddress + 2);
        ASSERT_EQ(results[1], thirdMemoryRegionAddress + 2);
    }

    TEST(MemoryScanner, ProperlyScansWithUnknownBytes) {
        MemoryScannerNew memoryScanner{};
        std::vector<BYTE> byteText{0x00, 0x01, 0x05, 0x10, 0xFC};
        size_t allocationSize = 0x3000;
        uintptr_t allocationAddress = reinterpret_cast<uintptr_t>(VirtualAlloc(nullptr, allocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
        uintptr_t endAddress = allocationAddress + allocationSize;
        if(!allocationAddress)
            FAIL("Couldn't allocate memory with VirtualAlloc");

        DWORD flOldProtect;
        LPVOID protectAddress = reinterpret_cast<LPVOID>(allocationAddress + 0x1000);
        VirtualProtect(protectAddress, 0x1000, PAGE_NOACCESS, &flOldProtect);

        uintptr_t thirdMemoryRegionAddress = allocationAddress + 0x2000;

        std::memcpy((LPVOID)allocationAddress, &byteText[0], byteText.size());
        std::memcpy((LPVOID)thirdMemoryRegionAddress, &byteText[0], byteText.size());

        auto results = memoryScanner.Scan(allocationAddress, endAddress, "05 ?? FC");
        ASSERT_EQ(results.size(), 2);
        ASSERT_EQ(results[0], allocationAddress + 2);
        ASSERT_EQ(results[1], thirdMemoryRegionAddress + 2);
    }
    TEST(MemoryScanner, GetsReadableMemoryRegions) {
        MemoryScannerNew memoryScanner{};

        size_t allocationSize = 0x3000;
        uintptr_t allocationAddress = reinterpret_cast<uintptr_t>(VirtualAlloc(nullptr, allocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
        uintptr_t endAddress = allocationAddress + allocationSize;
        if(!allocationAddress)
            FAIL("Couldn't allocate memory with VirtualAlloc");

        DWORD flOldProtect;
        LPVOID protectAddress = reinterpret_cast<LPVOID>(allocationAddress + 0x1000);
        VirtualProtect(protectAddress, 0x1000, PAGE_NOACCESS, &flOldProtect);

        uintptr_t thirdMemoryRegionAddress = allocationAddress + 0x2000;

        auto memoryRegions = memoryScanner.GetReadableMemoryRegions(allocationAddress, endAddress);
        EXPECT_EQ(memoryRegions.size(), 2);
    }

    TEST(MemoryScanner, QueriesVirtualMemory) {
        MemoryScannerNew memoryScanner{};

        size_t allocationSize = 0x5000;
        uintptr_t allocationAddress = reinterpret_cast<uintptr_t>(VirtualAlloc(nullptr, allocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

        auto memoryInformation = memoryScanner.QueryVirtualMemory(allocationAddress);
        EXPECT_EQ((uintptr_t)memoryInformation.AllocationBase, allocationAddress);
        EXPECT_EQ((uintptr_t)memoryInformation.BaseAddress, allocationAddress);
        EXPECT_EQ(memoryInformation.RegionSize, allocationSize);
        EXPECT_EQ(memoryInformation.Protect, PAGE_EXECUTE_READWRITE);
        EXPECT_EQ(memoryInformation.AllocationProtect, PAGE_EXECUTE_READWRITE);
        EXPECT_EQ(memoryInformation.State, MEM_COMMIT);
    }

    TEST(MemoryScanner, IsMemoryRegionReadable) {
        MemoryScannerNew memoryScanner{};

        size_t allocationSize = 0x5000;
        uintptr_t allocationAddress = reinterpret_cast<uintptr_t>(VirtualAlloc(nullptr, allocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
        bool isMemoryRegionReadable = memoryScanner.IsMemoryRegionReadable(allocationAddress);

        ASSERT_EQ(isMemoryRegionReadable, true);
        BYTE* pByte = reinterpret_cast<BYTE*>(allocationAddress);
        pByte[0] = 0xFA;

        DWORD oldProtect;
        VirtualProtect((LPVOID)allocationAddress, allocationSize, PAGE_NOACCESS, &oldProtect);
        isMemoryRegionReadable = memoryScanner.IsMemoryRegionReadable(allocationAddress);
        ASSERT_EQ(isMemoryRegionReadable, false);
    }

    TEST(MemoryScanner, IsMemoryRegionUsed) {
        MemoryScannerNew memoryScanner{};

        size_t allocationSize = 0x5000;
        uintptr_t allocationAddress = reinterpret_cast<uintptr_t>(VirtualAlloc(nullptr, allocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
        bool isMemoryRegionUsed = memoryScanner.IsMemoryRegionUsed(allocationAddress);
        ASSERT_EQ(isMemoryRegionUsed, true);

        bool decommitted = VirtualFree((LPVOID)allocationAddress, 0, MEM_DECOMMIT);
        isMemoryRegionUsed = memoryScanner.IsMemoryRegionUsed(allocationAddress);
        ASSERT_EQ(isMemoryRegionUsed, false);
    }
}
