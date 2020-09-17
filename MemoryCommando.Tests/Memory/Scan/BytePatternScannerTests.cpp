// ReSharper disable CppMemberFunctionMayBeConst
#include "BytePatternScannerTests.h"

#include <minwindef.h>
#include <vector>

#include "MemoryCommando/Memory/Scan/BytePatternScanner.h"
#include "MemoryCommando/Memory/Scan/ScanHelperMethods.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MemoryCommandoTests {
    using namespace MemoryCommando::Memory;

    BytePatternScannerTests::BytePatternScannerTests() : _bytePatternScanner(BytePatternScanner()) {
    }

    void BytePatternScannerTests::GenerateBadByteTable() {
        const std::string patternString = "01 ?? D0 43 ?? 50 AB ?? CD 50 24 1A 0A 08 04 03 07 09";

        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(patternString);
        auto badByteTable = _bytePatternScanner.GenerateBadByteTable(indexedPattern);

        for(const auto& indexedPatternItem : indexedPattern) {
            const auto badByteTableValue = badByteTable[indexedPatternItem.second];

            if(indexedPatternItem.first < 7 && badByteTableValue != 7) {
                const auto badTableByte = std::find_if(indexedPattern.begin(), indexedPattern.end(), [badByteTableValue](auto item) { return item.first == badByteTableValue; });
                if(badTableByte->second != indexedPatternItem.second)
                    Assert::IsTrue(false);
            }
            if(indexedPatternItem.first > 7 && badByteTableValue != indexedPatternItem.first)
                Assert::IsTrue(false);
        }

        Assert::IsTrue(true);
    }

    void BytePatternScannerTests::ScanIndexedBytePattern() {
        const std::vector<BYTE> byteSequence{ 0x43, 0x5C, 0xCA, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC, 0x62, 0xA0, 0x35, 0x34, 0x81 };
        const std::vector<std::pair<size_t, BYTE>> indexedPattern{ { 0, 0x18 }, {1, 0x33}, {3, 0xF0}, {4, 0x18}, {5, 0x33} };

        std::vector<size_t> scanResults = _bytePatternScanner.Scan(byteSequence, indexedPattern);

        if(scanResults.size() != 2)
            Assert::IsTrue(false);
        if(scanResults[0] != 3)
            Assert::IsTrue(false);
        if(scanResults[1] != 7)
            Assert::IsTrue(false);

        Assert::IsTrue(true);
    }

    void BytePatternScannerTests::ScanBytePattern() {
        const std::vector<BYTE> byteSequence{ 0x43, 0x5C, 0xCA, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC, 0x62, 0xA0, 0x35, 0x34, 0x81 };
        const std::vector<BYTE> bytePattern{ 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33 };

        std::vector<size_t> scanResults = _bytePatternScanner.Scan(byteSequence, bytePattern);

        if(scanResults.size() != 2)
            Assert::IsTrue(false);
        if(scanResults[0] != 3)
            Assert::IsTrue(false);
        if(scanResults[1] != 7)
            Assert::IsTrue(false);

        Assert::IsTrue(true);
    }

    void BytePatternScannerTests::ScanStringPattern() {
        const std::vector<BYTE> byteSequence{ 0x43, 0x5C, 0xCA, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC, 0x62, 0xA0, 0x35, 0x34, 0x81 };
        const std::string pattern = "0x18 0x33 ?? 0xF0 0x18 0x33";
        std::vector<size_t> scanResults = _bytePatternScanner.Scan(byteSequence, pattern);

        if(scanResults.size() != 2)
            Assert::IsTrue(false);
        if(scanResults[0] != 3)
            Assert::IsTrue(false);
        if(scanResults[1] != 7)
            Assert::IsTrue(false);

        Assert::IsTrue(true);
    }

    void BytePatternScannerTests::BigPatternScan() {
    }
}