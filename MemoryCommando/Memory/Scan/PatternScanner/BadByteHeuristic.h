#pragma once

#include <activation.h>
#include <utility>
#include <vector>
#include <gtest/gtest_prod.h>

#include "Memory/Scan/PatternScanner/BytePattern.h"

namespace MemoryCommando::Memory {
    class BadByteHeuristic {
        FRIEND_TEST(BadByteHeuristicF, GivenBytePatternWithNoUnknownBytes_PopulatesAllBytesNotInPatternWithMinusOne);
        FRIEND_TEST(BadByteHeuristicF, GivenBytePatternWithUnknownBytes_PopulatesAllBytesNotInPatternWithLastOccurence);
        FRIEND_TEST(BadByteHeuristicF, GivenBytePatternWithNoUnknownBytes_PopulatesBytesInPatternWithTheirIndex);
        FRIEND_TEST(BadByteHeuristicF, DeterminesLastUnknownByteNotAtTheEnd);
        FRIEND_TEST(BadByteHeuristicF, DeterminesLastUnknownByteAtTheEnd);
    public:
        BadByteHeuristic() = default;
        BadByteHeuristic(BytePattern bytePattern);
        long long GetShiftSuggestion(BYTE memoryByte, uint32_t memoryByteOffset);
    private:
        void PopulateBadByteTable();
        long long GetLastUnknownByteIndex() const;

        BytePattern bytePattern;
        static const int charMembersNumber = UCHAR_MAX + 1;
        long long badByteTable[charMembersNumber]{};
    };
}

