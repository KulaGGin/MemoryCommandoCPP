#include "PatternConverter.h"

namespace MemoryCommando::Memory {
    std::vector<std::pair<uint32_t, BYTE>> PatternConverter::ToByteSequence(const std::string& stringPattern) {
        std::vector<std::string> patternMembers = SplitPatternToPatternMembers(stringPattern);
        std::vector<std::pair<uint32_t, BYTE>> bytePattern = ConvertPatternMembersToBytePattern(patternMembers);
        return bytePattern;
    }

    std::vector<std::string> PatternConverter::SplitPatternToPatternMembers(const std::string& stringPattern) {
        std::vector<std::string> splitPattern;
        std::string delimiter = " ";

        size_t start = 0U;
        auto end = stringPattern.find(delimiter);

        while(end != std::string::npos) {
            splitPattern.push_back(stringPattern.substr(start, end - start));
            start = end + delimiter.length();
            end = stringPattern.find(delimiter, start);
        }

        splitPattern.push_back(stringPattern.substr(start, end - start));

        return splitPattern;
    }

    std::vector<std::pair<uint32_t, BYTE>> PatternConverter::ConvertPatternMembersToBytePattern(const std::vector<std::string>& patternMembers) const {
        std::vector<std::pair<uint32_t, BYTE>> bytePattern;

        for(size_t index = 0; index < patternMembers.size(); ++index) {
            const auto& patternMember = patternMembers[index];
            if(patternMember.size() > 2) {
                std::stringstream errorMessage;
                errorMessage << "Pattern member: " << patternMember << " has more than 2 characters in it.";
                throw std::runtime_error(errorMessage.str());
            }
            if(patternMember == "??")
                continue;

            BYTE byte = static_cast<BYTE>(std::stoi(patternMember, nullptr, 16));

            bytePattern.emplace_back(index, byte);
        }

        return bytePattern;
    }
}
