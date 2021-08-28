#include "gtest/gtest.h"

#include "Memory/Scan/BytePattern.h"

namespace MemoryCommando::Memory {
    TEST(BytePattern, CanInitialize) {
        BytePattern bytePattern{};
    }

    TEST(BytePattern, GivenPatternWithNoUnknownBytesGeneratesBytePatternWithProperSize) {
        BytePattern bytePattern("05 10 FC");
        auto byteSequence = bytePattern.GetPattern();

        EXPECT_EQ(bytePattern.GetSize(), 3);
    }
    TEST(BytePattern, GivenPatternWithNoUnknownBytesGeneratesBytePatternWithProperMembers) {
        BytePattern bytePattern("05 10 FC");
        auto byteSequence = bytePattern.GetPattern();

        EXPECT_EQ(byteSequence[0].first, 0);
        EXPECT_EQ(byteSequence[1].first, 1);
        EXPECT_EQ(byteSequence[2].first, 2);

        EXPECT_EQ(byteSequence[0].second, 0x05);
        EXPECT_EQ(byteSequence[1].second, 0x10);
        EXPECT_EQ(byteSequence[2].second, 0xFC);
    }

    TEST(BytePattern, GivenPatternWithUnknownBytesGeneratesBytePatternWithProperSize) {
        BytePattern bytePattern("05 ?? FC");
        auto byteSequence = bytePattern.GetPattern();

        EXPECT_EQ(bytePattern.GetSize(), 3);
    }

    TEST(BytePattern, GivenPatternWithUnknownBytes_GeneratesBytePatternWithProperMembers) {
        BytePattern bytePattern("05 ?? FC");
        auto byteSequence = bytePattern.GetPattern();

        EXPECT_EQ(byteSequence.size(), 2);
        EXPECT_EQ(byteSequence[0].first, 0);
        EXPECT_EQ(byteSequence[1].first, 2);

        EXPECT_EQ(byteSequence[0].second, 0x05);
        EXPECT_EQ(byteSequence[1].second, 0xFC);
    }

    TEST(BytePattern, GivenPatternWithUnknownBytesAtTheEnd_GeneratesBytePatternWithProperSize) {
        BytePattern bytePattern("05 ?? FC ?? ??");
        auto byteSequence = bytePattern.GetPattern();

        EXPECT_EQ(bytePattern.GetSize(), 5);
    }

    TEST(BytePattern, GivenPatternWithUnknownBytesAtTheEnd_GeneratesBytePatternWithProperMembers) {
        BytePattern bytePattern("05 ?? FC ?? ??");
        std::vector<std::pair<uint32_t, BYTE>> byteSequence = bytePattern.GetPattern();

        EXPECT_EQ(byteSequence.size(), 2);

        EXPECT_EQ(byteSequence[0].first, 0);
        EXPECT_EQ(byteSequence[1].first, 2);

        EXPECT_EQ(byteSequence[0].second, 0x05);
        EXPECT_EQ(byteSequence[1].second, 0xFC);
    }

    TEST(BytePattern, GivenByteSequence_GeneratesPatternWithProperSize) {
        BytePattern bytePattern(std::vector<BYTE>{0x05, 0x10, 0xFC});

        EXPECT_EQ(bytePattern.GetSize(), 3);
    }

    TEST(BytePattern, GivenByteSequence_GeneratesCorrectPattern) {
        BytePattern bytePattern(std::vector<BYTE>{0x05, 0x10, 0xFC});
        auto indexedBytePattern = bytePattern.GetPattern();

        EXPECT_EQ(indexedBytePattern[0].first, 0);
        EXPECT_EQ(indexedBytePattern[1].first, 1);
        EXPECT_EQ(indexedBytePattern[2].first, 2);

        EXPECT_EQ(indexedBytePattern[0].second, 0x05);
        EXPECT_EQ(indexedBytePattern[1].second, 0x10);
        EXPECT_EQ(indexedBytePattern[2].second, 0xFC);
    }
}