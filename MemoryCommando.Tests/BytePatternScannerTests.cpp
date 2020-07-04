#include <windows.h>

#include "BytePatternScannerTests.h"

#include <minwindef.h>
#include <vector>
#include <boost/algorithm/string.hpp>

#include "../MemoryCommando/Memory/BytePatternScanner.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MemoryCommandoTests {
    using namespace MemoryCommando::Memory;

    BytePatternScannerTests::BytePatternScannerTests() {
        _bytePatternScanner = BytePatternScanner();
    }

    void BytePatternScannerTests::GetLastWildcardIndex() {
        size_t lastWildcardIndex{};
        std::vector<std::string> patternStringSequence;

        std::string patternString = "01 FF D0 43";
        boost::split(patternStringSequence, patternString, boost::is_any_of(" "));
        lastWildcardIndex = _bytePatternScanner.GetLastWildcardIndex(patternStringSequence);
        if (lastWildcardIndex != 0)
            Assert::IsTrue(false);

        patternString = "?? FF D0 43";
        boost::split(patternStringSequence, patternString, boost::is_any_of(" "));
        lastWildcardIndex = _bytePatternScanner.GetLastWildcardIndex(patternStringSequence);
        if (lastWildcardIndex != 0)
            Assert::IsTrue(false);


        patternString += " ?? 50 AB BC CD";
        boost::split(patternStringSequence, patternString, boost::is_any_of(" "));
        lastWildcardIndex = _bytePatternScanner.GetLastWildcardIndex(patternStringSequence);
        if (lastWildcardIndex != 4)
            Assert::IsTrue(false);


        patternString += " ??";
        boost::split(patternStringSequence, patternString, boost::is_any_of(" "));
        lastWildcardIndex = _bytePatternScanner.GetLastWildcardIndex(patternStringSequence);
        if (lastWildcardIndex != patternStringSequence.size() - 1)
            Assert::IsTrue(false);



        Assert::IsTrue(true);
    }

    void BytePatternScannerTests::GetFilteredPattern() {
        std::string patternString = "01 ?? D0 43 ?? 50 AB ?? CD";
        std::vector<std::string> patternStringSequence;
        boost::split(patternStringSequence, patternString, boost::is_any_of(" "));
        auto filteredPattern = _bytePatternScanner.GetFilteredPattern(patternStringSequence);

        for(const auto& pair : filteredPattern) {
            if (pair.second != std::stoi(patternStringSequence[pair.first], nullptr, 16))
                Assert::IsTrue(false);
        }

        Assert::IsTrue(true);
    }

    void BytePatternScannerTests::GenerateBadByteTable() {
        std::string patternString = "01 ?? D0 43 ?? 50 AB ?? CD 02 24 1A 0A 08 04 03 07 09";
        std::vector<std::string> patternStringSequence;
        boost::split(patternStringSequence, patternString, boost::is_any_of(" "));
        const auto lastWildcardIndex = _bytePatternScanner.GetLastWildcardIndex(patternStringSequence);
        auto badByteTable = _bytePatternScanner.GenerateBadByteTable(patternStringSequence);

        for(size_t index = 0; index < patternStringSequence.size(); ++index) {
            if(patternStringSequence[index] == "??")
                continue;
                

            const size_t byteLastOccurence = index;
            const BYTE byteValue = std::stoi(patternStringSequence[index], nullptr, 16);

            if(byteLastOccurence < lastWildcardIndex && badByteTable[byteValue] != lastWildcardIndex)
                Assert::IsTrue(false);

            if (byteLastOccurence > lastWildcardIndex && byteLastOccurence != badByteTable[byteValue])
                Assert::IsTrue(false);
        }

        Assert::IsTrue(true);
    }

    void BytePatternScannerTests::Scan() {
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
}