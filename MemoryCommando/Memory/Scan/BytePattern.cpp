#include "BytePattern.h"

namespace MemoryCommando::Memory {

    BytePattern::BytePattern(const std::string& pattern) {
        auto patternMembers = patternConverter.SplitPatternToPatternMembers(pattern);
        size = static_cast<uint32_t>(patternMembers.size());
        indexedPattern = patternConverter.ConvertPatternMembersToBytePattern(patternMembers);
    }

    BytePattern::BytePattern(const std::vector<BYTE>& byteSequence) {
        indexedPattern = patternConverter.ConvertByteSequenceToIndexedBytePattern(byteSequence);
        size = indexedPattern.size();
    }

    uint32_t BytePattern::GetSize() const {
        return size;
    }

    std::vector<std::pair<uint32_t, BYTE>> BytePattern::GetPattern() const {
        return indexedPattern;
    }

    std::pair<uint32_t, BYTE>& BytePattern::operator[](int index) {
        return indexedPattern[index];
    }
}
