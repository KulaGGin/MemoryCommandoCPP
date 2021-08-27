#pragma once

#include <activation.h>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace MemoryCommando::Memory {
    class PatternConverter {
    public:
        std::vector<std::pair<uint32_t, BYTE>> ToByteSequence(const std::string& stringPattern);

        std::vector<std::string> SplitPatternToPatternMembers(const std::string& stringPattern);

        std::vector<std::pair<uint32_t, BYTE>> ConvertPatternMembersToBytePattern(const std::vector<std::string>& patternMembers) const;
    };
}
