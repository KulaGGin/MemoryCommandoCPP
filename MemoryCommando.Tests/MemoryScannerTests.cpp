#include "../MemoryCommando/Memory/BytePatternScanner.h"

#include "MemoryScannerTests.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MemoryCommandoTests {
    using namespace MemoryCommando::Memory;

    MemoryScannerTests::MemoryScannerTests() {
    }

    void MemoryScannerTests::GetMemoryRegions() {
        auto address = _memoryManager.AllocateVirtualMemory(0, 0x5000);
        auto memoryRegions = _memoryScanner.GetMemoryRegions(address, address + 0x5000);
        if(memoryRegions.size() != 1)
            Assert::IsTrue(false);
        if(uintptr_t(memoryRegions[0].BaseAddress) != address)
            Assert::IsTrue(false);
        if(memoryRegions[0].RegionSize != 0x5000)
            Assert::IsTrue(false);

        _memoryManager.ProtectVirtualMemory(address + 0x0000, 0x1000, PAGE_READONLY);
        _memoryManager.ProtectVirtualMemory(address + 0x1000, 0x1000, PAGE_READWRITE);
        _memoryManager.ProtectVirtualMemory(address + 0x2000, 0x1000, PAGE_NOACCESS);

        memoryRegions = _memoryScanner.GetMemoryRegions(address, address + 0x5000);

        if(memoryRegions.size() != 4)
            Assert::IsTrue(false);
        if(uintptr_t(memoryRegions[0].BaseAddress) != address || memoryRegions[0].RegionSize != 0x1000)
            Assert::IsTrue(false);
        if(uintptr_t(memoryRegions[1].BaseAddress) != address + 0x1000 || memoryRegions[1].RegionSize != 0x1000)
            Assert::IsTrue(false);
        if(uintptr_t(memoryRegions[2].BaseAddress) != address + 0x2000 || memoryRegions[2].RegionSize != 0x1000)
            Assert::IsTrue(false);
        if(uintptr_t(memoryRegions[3].BaseAddress) != address + 0x3000 || memoryRegions[3].RegionSize != 0x2000)
            Assert::IsTrue(false);

        Assert::IsTrue(true);
    }

    void MemoryScannerTests::Scan() {
        std::vector<BYTE> byteArray{ 0x43, 0x5C, 0xCA, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC, 0x62, 0xA0, 0x35, 0x34, 0x81 };
        const uintptr_t byteArrayAddress = uintptr_t(&byteArray[0]);
        const uintptr_t addressPastTheEnd = uintptr_t(&byteArray[0] + byteArray.size());
        const std::string pattern = "0x18 0x33 ?? 0xF0 0x18 0x33";

        std::vector<uintptr_t> patternAddresses = _memoryScanner.Scan(byteArrayAddress - 4, addressPastTheEnd + 4, pattern);
        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != byteArrayAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != byteArrayAddress + 7)
            Assert::IsTrue(false);

        Assert::IsTrue(true);
    }
}
