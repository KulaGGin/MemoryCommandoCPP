#include <activation.h>
#include <iostream>

#include "gtest/gtest.h"

#include "Memory/Scan/PatternScanner/PatternConverter.h"

namespace MemoryCommando::Memory {
    TEST(PatternConverter, ConvertsStringPatternToByteSequence) {
        PatternConverter patternConverter{};

        auto bytesFromConverter = patternConverter.ToByteSequence("00 01 02");
        std::vector<BYTE> convertedBytes = {0x0, 0x1, 0x2};

        ASSERT_EQ(bytesFromConverter.size(), 3);
        ASSERT_EQ(bytesFromConverter[0].second, convertedBytes[0]);
        ASSERT_EQ(bytesFromConverter[1].second, convertedBytes[1]);
        ASSERT_EQ(bytesFromConverter[2].second, convertedBytes[2]);
    }

    TEST(PatternConverter, ConvertsStringPatternTyPatternMembers) {
        std::string stringPattern{"00 01 02"};
        PatternConverter patternConverter{};
        std::vector<std::string> patternMembers = patternConverter.SplitPatternToPatternMembers(stringPattern);

        ASSERT_EQ(patternMembers[0], "00");
        ASSERT_EQ(patternMembers[1], "01");
        ASSERT_EQ(patternMembers[2], "02");
    }

    TEST(PatternConverter, ConvertsPatternMembersToBytePattern) {
        std::vector<std::string> patternMembers{"05", "10", "FC"};
        PatternConverter patternConverter{};
        std::vector<std::pair<uint32_t, BYTE>> bytePattern = patternConverter.ConvertPatternMembersToBytePattern(patternMembers);

        ASSERT_EQ(bytePattern.size(), 3);

        ASSERT_EQ(bytePattern[0].first, 0);
        ASSERT_EQ(bytePattern[1].first, 1);
        ASSERT_EQ(bytePattern[2].first, 2);

        ASSERT_EQ(bytePattern[0].second, 0x05);
        ASSERT_EQ(bytePattern[1].second, 0x10);
        ASSERT_EQ(bytePattern[2].second, 0xFC);
    }
}