#define NOMINMAX

#include "BytePatternScanner.h"
#include <boost/algorithm/string.hpp>
#include <map>
#include <algorithm>

namespace MemoryCommando::Scan {

    std::vector<size_t> BytePatternScanner::Scan(std::vector<BYTE> byteSequence, std::string pattern) {
        std::vector<size_t> patternIndexes{};

        patternIndexes = GetPatternIndexes(byteSequence, pattern);

        return patternIndexes;
    }

    size_t BytePatternScanner::GetLastWildcardIndex(const std::vector<std::string>& patternStringSequence) {
        // Initialize lastWildcardIndex to 0 in case there are no wildcards in the pattern
        size_t lastWildcardIndex;

        // Get last index of ?? wildcard
        size_t index = patternStringSequence.size() - 1;
        while(true) {
            if(patternStringSequence[index] == "??" || index == 0) {
                lastWildcardIndex = index;
                break;
            }

            --index;
        }

        return lastWildcardIndex;
    }

    std::vector<std::pair<size_t, BYTE>> BytePatternScanner::GetFilteredPattern(std::vector<std::string> pattern) {
        std::vector<std::pair<size_t, BYTE>> filteredPattern;

        for(size_t index = 0; index < pattern.size(); ++index) {
            if(pattern[index] == "??")
                continue;

            BYTE value = std::stoi(pattern[index], nullptr, 16);
            filteredPattern.emplace_back(index, value);
        }

        return filteredPattern;
    }

    std::map<int, size_t> BytePatternScanner::GenerateBadByteTable(const std::vector<std::string>& patternStringSequence) {
        constexpr int ByteQuantity = MAXBYTE + 1;
        std::map<int, size_t> badByteTable{};
        auto lastWildcardIndex = GetLastWildcardIndex(patternStringSequence);

        // Initialize all occurrences as last wildcard index
        // lastWildcardIndex value is used to calculate the value by which to shift the pattern by default
        for(size_t index = 0; index < ByteQuantity; ++index) {
            badByteTable.insert(std::pair(index, lastWildcardIndex));
        }

        // process only bytes that are after the last wildcard index because minimum index we will use to shift the pattern is wildcard index
        // because any byte can take wildcard index place
        for (size_t byteLastOccurence = lastWildcardIndex + 1; byteLastOccurence < patternStringSequence.size(); ++byteLastOccurence) {
            const BYTE byteValue = std::stoi(patternStringSequence[byteLastOccurence], nullptr, 16);
            badByteTable[byteValue] = byteLastOccurence;
        }

        return badByteTable;
    }

    std::vector<size_t> BytePatternScanner::GetPatternIndexes(const std::vector<BYTE>& byteSequence, std::string pattern) {
        std::vector<size_t> patternIndexes{};
        // getting pattern ready for scanning
        std::vector<std::string> patternStringSequence;
        boost::split(patternStringSequence, pattern, boost::is_any_of(" "));

        const auto filteredPattern = GetFilteredPattern(patternStringSequence);
        auto badByteTable = GenerateBadByteTable(patternStringSequence);
        const auto lastByteIndex = filteredPattern[filteredPattern.size() - 1].first;

        // loop until pattern will overflow over the end of byte sequence.
        size_t offsetIndex = 0; // shift offset of the pattern with respect to byte sequence.
        while(offsetIndex < byteSequence.size() - lastByteIndex) {

            // Check the pattern at the current offset in the byte sequence
            // loop as long as we have a match between byte in the byte sequence and pattern byte
            int filteredPatternIndex = filteredPattern.size(); // start checking the pattern from the end
            while(true) {
                filteredPatternIndex--;
                const size_t fullPatternIndex = filteredPattern[filteredPatternIndex].first;
                const size_t byteSequenceIndex = offsetIndex + fullPatternIndex;
                const BYTE patternByte = filteredPattern[filteredPatternIndex].second;
                const BYTE byteSequenceByte = byteSequence[byteSequenceIndex];

                if(patternByte != byteSequenceByte) { // on a mismatch
                    // Shift the pattern so that the bad byte in byte array aligns with the last occurrence of it in the pattern.
                    // The max function is used to  make sure that we get a positive shift.
                    // We may get a negative shift if the last occurrence of bad byte in the pattern is on the right side of the current  character.
                    const auto byteShiftNumber = std::max(unsigned(1), fullPatternIndex - badByteTable[byteSequenceByte]);
                    offsetIndex += byteShiftNumber;
                    break;
                }
                // if it's a match
                if(filteredPatternIndex == 0) { // and it's last byte in the filtered pattern of bytes
                    patternIndexes.push_back(offsetIndex);

                    // Shift the pattern so that the next byte in the byte array aligns with the last occurrence of it in pattern.
                    // The condition memoryOffset+fullPatternLength < memoryRegion.Length is necessary for the case when pattern occurs at the end of text.
                    const auto byteShiftNumber = std::max(SSIZE_T(1), SSIZE_T(fullPatternIndex - badByteTable[byteSequenceByte]));
                    offsetIndex += byteShiftNumber;
                    break;
                }
            }
        }

        return patternIndexes;
    }
}
