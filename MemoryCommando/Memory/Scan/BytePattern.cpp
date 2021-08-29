#include "BytePattern.h"

namespace MemoryCommando::Memory {

    BytePattern::BytePattern(const std::string& pattern) {
        PatternMembersSequence patternMembers = patternConverter.SplitPatternToPatternMembers(pattern);
        size = static_cast<uint32_t>(patternMembers.size());
        indexedPattern = patternConverter.ConvertPatternMembersToIndexedBytePattern(patternMembers);
    }

    BytePattern::BytePattern(const ByteSequence& byteSequence) {
        indexedPattern = patternConverter.ConvertByteSequenceToIndexedBytePattern(byteSequence);
        size = indexedPattern.size();
    }

    uint32_t BytePattern::GetSize() const {
        return size;
    }

    IndexedBytePattern BytePattern::GetIndexedPattern() const {
        return indexedPattern;
    }

    IndexedByte& BytePattern::operator[](int index) {
        return indexedPattern[index];
    }
}
