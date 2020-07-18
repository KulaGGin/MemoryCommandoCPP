#define NOMINMAX

#include "BytePatternScanner.h"
#include <boost/algorithm/string.hpp>
#include <map>
#include <algorithm>
#include "ScanHelperMethods.h"

namespace MemoryCommando::Memory {

    std::vector<size_t> BytePatternScanner::Scan(const std::vector<BYTE>& byteSequence, const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const {
        std::vector<size_t> patternIndexes{};

        std::vector<size_t> badByteTable = GenerateBadByteTable(indexedPattern);
        const auto lastByteIndex = indexedPattern[indexedPattern.size() - 1].first;

        // loop until pattern will overflow over the end of byte sequence.
        size_t offsetIndex = 0; // shift offset of the pattern with respect to byte sequence.
        while(offsetIndex < byteSequence.size() - lastByteIndex) {

            // Check the pattern at the current offset in the byte sequence
            // loop as long as we have a match between byte in the byte sequence and pattern byte
            size_t filteredPatternIndex = indexedPattern.size(); // start checking the pattern from the end
            while(true) {
                filteredPatternIndex--;
                const size_t fullPatternIndex = indexedPattern[filteredPatternIndex].first;
                const size_t byteSequenceIndex = offsetIndex + fullPatternIndex;
                const BYTE patternByte = indexedPattern[filteredPatternIndex].second;
                const BYTE byteSequenceByte = byteSequence[byteSequenceIndex];

                // todo get rid of code duplication
                if(patternByte != byteSequenceByte) { // on a mismatch
                    // Shift the pattern so that the bad byte in byte array aligns with the last occurrence of it in the pattern.
                    // The max function is used to  make sure that we get a positive shift.
                    // We may get a negative shift if the last occurrence of bad byte in the pattern is on the right side of the current  character.
                    const auto byteShiftNumber = std::max(SSIZE_T(1), SSIZE_T(fullPatternIndex - badByteTable[byteSequenceByte]));
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

    std::vector<size_t> BytePatternScanner::Scan(const std::vector<BYTE>& byteSequence, const std::vector<BYTE>& bytePattern) const {
        // getting pattern ready for scanning
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(bytePattern);

        std::vector<size_t> patternIndexes = Scan(byteSequence, indexedPattern);

        return patternIndexes;
    }

    std::vector<size_t> BytePatternScanner::Scan(const std::vector<BYTE>& byteSequence, const std::string& stringPattern) const {
        // getting pattern ready for scanning
        const auto indexedPattern = ScanHelperMethods::GetIndexedPattern(stringPattern);

        std::vector<size_t> patternIndexes = Scan(byteSequence, indexedPattern);

        return patternIndexes;
    }

    std::vector<size_t> BytePatternScanner::GenerateBadByteTable(const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const {
        std::vector<size_t> badByteTable{};
        auto lastConsecutiveIterator = indexedPattern.begin();

        //Get last consecutive byte item from the end
                //Get last consecutive byte item from the end
        for(auto currentIterator = indexedPattern.rbegin(); currentIterator != indexedPattern.rend() - 1; ++currentIterator) {
            if(currentIterator->first - (currentIterator + 1)->first != 1) {
                lastConsecutiveIterator = (currentIterator + 1).base();
                break;
            }
        }

        // Initialize all occurrences as last wildcard index
        // lastWildcardIndex value is used to calculate the value by which to shift the pattern by default
        for(size_t index = 0; index < _byteQuantity; ++index) {
            badByteTable.push_back(lastConsecutiveIterator->first - 1);
        }

        // process only bytes that are after the last wildcard index because minimum index we will use to shift the pattern is wildcard index
        // because any byte can take wildcard index place
        for(auto currentIterator = lastConsecutiveIterator; currentIterator != indexedPattern.end(); ++currentIterator) {
            const BYTE byteValue = currentIterator->second;
            const size_t byteLastOccurence = currentIterator->first;

            badByteTable[byteValue] = byteLastOccurence;
        }

        return badByteTable;
    }
}