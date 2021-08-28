#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "BadByteHeuristic.h"
#include "BytePattern.h"

namespace MemoryCommando::Memory {
    using ScanResults = std::vector<uintptr_t>;

    class MatchResult {
    public:
        bool FoundFullMatch = false;
        uint32_t MismatchIndex = 0;
    };

    class PatternScanner {
        FRIEND_TEST(PatternScanner, CorrectlyDeterminesIfPatternReachesPastEndAddress);
        FRIEND_TEST(PatternScanner, CorrectlyDeterminesIfOffsetReachesPastScanEndAddress);
        FRIEND_TEST(PatternScanner, DeterminesBadByteHeuristicOffset);
        FRIEND_TEST(PatternScanner, MatchesPatternWithMemory);
        FRIEND_TEST(PatternScanner, MatchResultIsTrueOnFullMatch);
    public:
        ScanResults Scan(uintptr_t startAddress, uintptr_t endAddress, std::string pattern);
    private:
        MatchResult MatchPatternWithMemory() const;
        uint32_t DetermineBadByteHeuristicOffset(const MatchResult& matchResult) const;
        bool DoesPatternReachPastEndOffset(uintptr_t currentMemoryOffset) const;
        bool DoesOffsetReachPastScanEndAddress(uintptr_t mMemoryOffset) const;

        BytePattern bytePattern{};
        BadByteHeuristic badByteHeuristic{};

        BYTE* memory_bytes{};
        uintptr_t memory_currentScanShift{};
        uintptr_t memory_maximumByteOffset{};
    };
}