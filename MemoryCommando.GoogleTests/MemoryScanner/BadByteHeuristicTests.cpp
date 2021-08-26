#include "gtest/gtest.h"
#include "Memory/Scan/PatternScanner/BadByteHeuristic.h"
#include "Memory/Scan/PatternScanner/BytePattern.h"


namespace MemoryCommando::Memory {
    using namespace testing;
    class BadByteHeuristicF : public Test {
    public:
        bool IsPatternByte(const BytePattern& bytePattern, BYTE byte) {
            bool isPatternByte = false;

            for(const auto& patternMember : bytePattern.GetPattern()) {
                if(byte == patternMember.second) {
                    isPatternByte = true;
                    break;
                }
            }

            return isPatternByte;
        }
    };
    TEST_F(BadByteHeuristicF, GivenMemoryByteAndOffsetProvidesWithAppropriateShiftSuggestion) {
        BytePattern patternToFind("05 10 FC");
        BadByteHeuristic badCharacterHeuristic(patternToFind);

        long long suggestedShiftOffset = badCharacterHeuristic.GetShiftSuggestion(0x05, 2);
        EXPECT_EQ(suggestedShiftOffset, 2);

        suggestedShiftOffset = badCharacterHeuristic.GetShiftSuggestion(0x01, 2);
        EXPECT_EQ(suggestedShiftOffset, 3);
    }
    TEST_F(BadByteHeuristicF, GivenBytePatternWithNoUnknownBytes_PopulatesAllBytesNotInPatternWithMinusOne) {
        BytePattern patternToFind("05 10 FC");
        BadByteHeuristic badCharacterHeuristic(patternToFind);

        const long long(&badByteTable)[256] = badCharacterHeuristic.badByteTable;

        for(size_t index = 0; index < ARRAYSIZE(badByteTable); ++index) {
            if(IsPatternByte(patternToFind, index))
                continue;

            EXPECT_EQ(badByteTable[index], -1);
        }
    }

    TEST_F(BadByteHeuristicF, GivenBytePatternWithUnknownBytes_PopulatesAllBytesNotInPatternWithLastOccurence) {
        BytePattern patternToFind("05 ?? ?? FC");
        BadByteHeuristic badCharacterHeuristic(patternToFind);

        const long long(&badByteTable)[256] = badCharacterHeuristic.badByteTable;

        for(size_t index = 0; index < ARRAYSIZE(badByteTable); ++index) {
            if(IsPatternByte(patternToFind, index))
                continue;

            EXPECT_EQ(badByteTable[index], 2);
        }
    }
    TEST_F(BadByteHeuristicF, GivenBytePatternWithNoUnknownBytes_PopulatesBytesInPatternWithTheirIndex) {
        BytePattern patternToFind("05 10 FC");
        BadByteHeuristic badCharacterHeuristic(patternToFind);

        const long long(&badByteTable)[256] = badCharacterHeuristic.badByteTable;

        for(const auto& [patternIndex, value] : patternToFind.GetPattern()) {
            long long badByteShiftIndex = badByteTable[value];

            EXPECT_EQ(badByteShiftIndex, patternIndex);
        }
    }

    TEST_F(BadByteHeuristicF, DeterminesLastUnknownByteNotAtTheEnd) {
        BytePattern patternToFind("05 ?? ?? FC");
        BadByteHeuristic badCharacterHeuristic(patternToFind);

        uint32_t lastUnknownByteIndex = badCharacterHeuristic.GetLastUnknownByteIndex();
        ASSERT_EQ(lastUnknownByteIndex, 2);
    }

    TEST_F(BadByteHeuristicF, DeterminesLastUnknownByteAtTheEnd) {
        BytePattern patternToFind("05 ?? ?? FC ?? ??");
        BadByteHeuristic badCharacterHeuristic(patternToFind);

        uint32_t lastUnknownByteIndex = badCharacterHeuristic.GetLastUnknownByteIndex();
        ASSERT_EQ(lastUnknownByteIndex, 5);
    }
}
