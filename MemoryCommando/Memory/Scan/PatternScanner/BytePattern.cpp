#include "BytePattern.h"

namespace MemoryCommando::Memory {

    BytePattern::BytePattern(const std::string& pattern) {
        auto patternMembers = patternConverter.SplitPatternToPatternMembers(pattern);
        size = static_cast<uint32_t>(patternMembers.size());
        bytePattern = patternConverter.ConvertPatternMembersToBytePattern(patternMembers);
    }

    uint32_t BytePattern::GetSize() const {
        return size;
    }

    std::vector<std::pair<uint32_t, BYTE>> BytePattern::GetPattern() const {
        return bytePattern;
    }

    std::pair<uint32_t, BYTE>& BytePattern::operator[](int index) {
        return bytePattern[index];
    }
}
