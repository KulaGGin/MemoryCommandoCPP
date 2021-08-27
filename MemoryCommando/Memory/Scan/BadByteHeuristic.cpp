#include "BadByteHeuristic.h"

namespace MemoryCommando::Memory {
    BadByteHeuristic::BadByteHeuristic(BytePattern bytePattern) {
        this->bytePattern = std::move(bytePattern);
        PopulateBadByteTable();
    }

    long long BadByteHeuristic::GetShiftSuggestion(BYTE memoryByte, uint32_t memoryByteOffset) {
        long long shiftSuggestion = memoryByteOffset - badByteTable[memoryByte];
        return shiftSuggestion;
    }

    void BadByteHeuristic::PopulateBadByteTable() {
        long long defaultLastOccurenceIndex = GetLastUnknownByteIndex();

        for(auto& badByte : badByteTable) {
            badByte = defaultLastOccurenceIndex;
        }

        for(const auto& [patternIndex, value] : bytePattern.bytePattern) {
            badByteTable[value] = patternIndex;
        }
    }

    long long BadByteHeuristic::GetLastUnknownByteIndex() const {
        uint32_t lastPatternMemberIndex = bytePattern.bytePattern.back().first;

        if(lastPatternMemberIndex < bytePattern.GetSize() - 1)
            return bytePattern.GetSize() - 1;

        for(long long index = bytePattern.bytePattern.size() - 1; index > 0; --index) {
            auto previousPatternMemberIndex = bytePattern.bytePattern[index - 1].first;
            auto currentPatternMemberIndex = bytePattern.bytePattern[index].first;
            if(previousPatternMemberIndex < currentPatternMemberIndex - 1)
                return currentPatternMemberIndex - 1;
        }

        return -1;
    }
}