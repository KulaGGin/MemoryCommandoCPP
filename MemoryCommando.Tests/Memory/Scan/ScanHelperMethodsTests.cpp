// ReSharper disable CppMemberFunctionMayBeConst
#include "ScanHelperMethodsTests.h"

#include <vector>

#include "MemoryCommando/Memory/Scan/ScanHelperMethods.h"

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>


namespace MemoryCommandoTests {
    using namespace Microsoft::VisualStudio::CppUnitTestFramework;
    using namespace MemoryCommando::Memory;

    void ScanHelperMethodsTests::GetIndexedPatternString() {
        std::string patternString = "01 ?? D0 43 ?? 50 AB ?? CD";
        std::vector<std::string> patternStringSequence;
        boost::split(patternStringSequence, patternString, boost::is_any_of(" "));
        auto indexedPattern = ScanHelperMethods::GetIndexedPattern(patternString);

        for(const auto& pair : indexedPattern) {
            if(pair.second != std::stoi(patternStringSequence[pair.first], nullptr, 16))
                Assert::IsTrue(false);
        }

        Assert::IsTrue(true);
    }

    void ScanHelperMethodsTests::GetIndexedPatternStringBytes() {
        std::string patternString = "01 ?? D0 43 ?? 50 AB ?? CD";
        std::vector<std::string> patternStringBytes;
        boost::split(patternStringBytes, patternString, boost::is_any_of(" "));
        auto indexedPattern = ScanHelperMethods::GetIndexedPattern(patternStringBytes);

        for(const auto& pair : indexedPattern) {
            if(pair.second != std::stoi(patternStringBytes[pair.first], nullptr, 16))
                Assert::IsTrue(false);
        }

        Assert::IsTrue(true);
    }

    void ScanHelperMethodsTests::GetIndexedPatternBytes() {
        const std::vector<BYTE> patternBytes{ 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33 };
        auto indexedPattern = ScanHelperMethods::GetIndexedPattern(patternBytes);

        for(const auto& pair : indexedPattern) {
            if(pair.second != patternBytes[pair.first])
                Assert::IsTrue(false);
        }

        Assert::IsTrue(true);
    }
}
