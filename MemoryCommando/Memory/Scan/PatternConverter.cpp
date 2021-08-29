#include "PatternConverter.h"

namespace MemoryCommando::Memory {
    IndexedBytePattern PatternConverter::StringToIndexedPattern(const StringPattern& stringPattern) {
        std::vector<std::string> patternMembers = SplitPatternToPatternMembers(stringPattern);
        IndexedBytePattern bytePattern = ConvertPatternMembersToIndexedBytePattern(patternMembers);
        return bytePattern;
    }

    PatternMembersSequence PatternConverter::SplitPatternToPatternMembers(const StringPattern& stringPattern) {
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

    IndexedBytePattern PatternConverter::ConvertPatternMembersToIndexedBytePattern(const PatternMembersSequence& patternMembers) const {
        IndexedBytePattern bytePattern;

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

    IndexedBytePattern PatternConverter::ConvertByteSequenceToIndexedBytePattern(const ByteSequence& byteSequence) const {
        IndexedBytePattern indexedBytePattern{};

        for(size_t currentIndex = 0; currentIndex < byteSequence.size(); ++currentIndex) {
            indexedBytePattern.emplace_back(currentIndex, byteSequence[currentIndex]);
        }

        return indexedBytePattern;
    }
}
