#include "ScanHelperMethods.h"

#include "HelperMethods.h"

namespace MemoryCommando::Memory {
    std::vector<std::pair<size_t, BYTE>> ScanHelperMethods::GetIndexedPattern(const std::vector<std::string>& pattern) {
        std::vector<std::pair<size_t, BYTE>> indexedPattern;

        for(size_t index = 0; index < pattern.size(); ++index) {
            if(pattern[index] == "??")
                continue;

            BYTE value = std::stoi(pattern[index], nullptr, 16);
            indexedPattern.emplace_back(index, value);
        }

        return indexedPattern;
    }

    std::vector<std::pair<size_t, BYTE>> ScanHelperMethods::GetIndexedPattern(const std::string& pattern) {
        const std::vector<std::string> patternSplitVector = HelperMethods::SplitString(pattern, ' ');

        std::vector<std::pair<size_t, BYTE>> indexedPattern = GetIndexedPattern(patternSplitVector);

        return indexedPattern;
    }

    std::vector<std::pair<size_t, BYTE>> ScanHelperMethods::GetIndexedPattern(const std::vector<BYTE>& pattern) {
        std::vector<std::pair<size_t, BYTE>> filteredPattern;

        for(size_t index = 0; index < pattern.size(); ++index) {
            BYTE value = pattern[index];
            filteredPattern.emplace_back(index, value);
        }

        return filteredPattern;
    }
}

