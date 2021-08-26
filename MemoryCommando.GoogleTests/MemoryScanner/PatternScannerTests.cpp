#include <activation.h>

#include "gtest/gtest.h"
#include "Memory/Scan/PatternScanner/PatternScanner.h"

namespace MemoryCommando::Memory {
    TEST(PatternScanner, CanBeInitialized) {
        PatternScanner memoryPatternScanner{};
    }

    TEST(PatternScanner, FindsPatternAndReturnsAddress) {
        PatternScanner patternScanner{};
        std::vector<BYTE> bytePattern{0x00, 0x01, 0x05, 0x10, 0xFC};
        uintptr_t startAddress = reinterpret_cast<uintptr_t>(&bytePattern.front());
        uintptr_t endAddress = reinterpret_cast<uintptr_t>(&bytePattern.back());

        ScanResults scanResults = patternScanner.Scan(startAddress, endAddress, "05 10 FC");
        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults[0], (uintptr_t)&bytePattern[2]);
    }

    TEST(PatternScanner, CorrectlyDeterminesIfPatternReachesPastEndAddress) {
        PatternScanner patternScanner{};
        std::vector<BYTE> bytePattern{0x00, 0x01, 0x05, 0x10, 0xFC};

        uintptr_t startAddress = reinterpret_cast<uintptr_t>(&bytePattern.front());
        uintptr_t endAddress = reinterpret_cast<uintptr_t>(&bytePattern.back());

        patternScanner.memory_bytes = (BYTE*)startAddress;
        patternScanner.memory_maximumByteOffset = endAddress;
        patternScanner.bytePattern = BytePattern("05 10 FC");

        bool doesPatternReachPastEndAddress = patternScanner.DoesPatternReachPastEndOffset(startAddress);
        ASSERT_EQ(doesPatternReachPastEndAddress, false);

        doesPatternReachPastEndAddress = patternScanner.DoesPatternReachPastEndOffset(startAddress + 2);
        ASSERT_EQ(doesPatternReachPastEndAddress, false);

        doesPatternReachPastEndAddress = patternScanner.DoesPatternReachPastEndOffset(startAddress + 3);
        ASSERT_EQ(doesPatternReachPastEndAddress, true);
    }

    TEST(PatternScanner, DeterminesBadByteHeuristicOffset) {
        PatternScanner patternScanner{};
        std::vector<BYTE> byteText{0x00, 0x01, 0x05, 0x10, 0xFC};

        uintptr_t startAddress = reinterpret_cast<uintptr_t>(&byteText.front());
        uintptr_t endAddress = reinterpret_cast<uintptr_t>(&byteText.back());

        patternScanner.memory_bytes = (BYTE*)startAddress;
        patternScanner.memory_currentScanShift = 0;
        patternScanner.memory_maximumByteOffset = endAddress;
        patternScanner.bytePattern = BytePattern("05 10 FC");
        MatchResult matchResult{false, 2};

        auto badByteHeuristicOffset = patternScanner.DetermineBadByteHeuristicOffset(matchResult);
        ASSERT_EQ(badByteHeuristicOffset, 2);

        matchResult.FoundFullMatch = true;
        badByteHeuristicOffset = patternScanner.DetermineBadByteHeuristicOffset(matchResult);
        ASSERT_EQ(badByteHeuristicOffset, patternScanner.bytePattern.GetSize());
    }

    TEST(PatternScanner, MatchesPatternWithMemory) {
        PatternScanner patternScanner{};
        std::vector<BYTE> byteText{0x00, 0x01, 0x05, 0xFC, 0xFC};

        uintptr_t startAddress = reinterpret_cast<uintptr_t>(&byteText.front());
        uintptr_t endAddress = reinterpret_cast<uintptr_t>(&byteText.back());

        patternScanner.memory_bytes = (BYTE*)startAddress;
        patternScanner.memory_currentScanShift = 0;
        patternScanner.memory_maximumByteOffset = endAddress;
        patternScanner.bytePattern = BytePattern("05 10 FC");

        MatchResult matchResult = patternScanner.MatchPatternWithMemory();
        EXPECT_EQ(matchResult.FoundFullMatch, false);
        EXPECT_EQ(matchResult.MismatchIndex, 2);

        patternScanner.memory_currentScanShift = 1;
        matchResult = patternScanner.MatchPatternWithMemory();
        EXPECT_EQ(matchResult.FoundFullMatch, false);
        EXPECT_EQ(matchResult.MismatchIndex, 1);
    }

    TEST(PatternScanner, MatchResultIsTrueOnFullMatch) {
        PatternScanner patternScanner{};
        std::vector<BYTE> byteText{0x00, 0x01, 0x05, 0x10, 0xFC};

        uintptr_t startAddress = reinterpret_cast<uintptr_t>(&byteText.front());
        uintptr_t endAddress = reinterpret_cast<uintptr_t>(&byteText.back());

        patternScanner.memory_bytes = (BYTE*)startAddress;
        patternScanner.memory_currentScanShift = 2;
        patternScanner.memory_maximumByteOffset = endAddress;
        patternScanner.bytePattern = BytePattern("05 10 FC");

        MatchResult matchResult = patternScanner.MatchPatternWithMemory();
        EXPECT_EQ(matchResult.FoundFullMatch, true);
    }
}
