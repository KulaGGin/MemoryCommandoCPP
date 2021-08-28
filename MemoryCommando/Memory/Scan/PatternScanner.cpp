#include "PatternScanner.h"

namespace MemoryCommando::Memory {
    ScanResults PatternScanner::Scan(uintptr_t startAddress, uintptr_t endAddress, std::string pattern) {
        ScanResults scanResults;
        bytePattern = BytePattern(pattern);
        badByteHeuristic = BadByteHeuristic(bytePattern);


        memory_bytes = reinterpret_cast<BYTE*>(startAddress);
        memory_currentScanShift = 0;
        memory_maximumByteOffset = endAddress - startAddress;

        while(!DoesPatternReachPastEndOffset(memory_currentScanShift)) {
            MatchResult matchResult = MatchPatternWithMemory();

            if(matchResult.FoundFullMatch)
                scanResults.push_back(startAddress + memory_currentScanShift);

            uint32_t badByteHeuristicOffset = DetermineBadByteHeuristicOffset(matchResult);
            if(DoesOffsetReachPastScanEndAddress(memory_currentScanShift + badByteHeuristicOffset))
                break;

            BYTE memoryByte = memory_bytes[memory_currentScanShift + badByteHeuristicOffset];
            long long badByteHeuristicShift = badByteHeuristic.GetShiftSuggestion(memoryByte, badByteHeuristicOffset);
            memory_currentScanShift += max(1, badByteHeuristicShift);
        }

        return scanResults;
    }

    MatchResult PatternScanner::MatchPatternWithMemory() const {
        BYTE* memoryAddressStart = memory_bytes + memory_currentScanShift;
        MatchResult matchResult{};
        size_t bytePattern_index = bytePattern.bytePattern.size() - 1;
        size_t currentByteMatchPatternIndex = bytePattern.bytePattern[bytePattern_index].first; // start from the end
        while(true) {
            BYTE patternByte = bytePattern.bytePattern[bytePattern_index].second;
            BYTE memoryByte = memoryAddressStart[currentByteMatchPatternIndex];

            if(patternByte != memoryByte) {
                matchResult.MismatchIndex = bytePattern.bytePattern[bytePattern_index].first;
                break;
            }

            if(bytePattern_index == 0) {
                matchResult.FoundFullMatch = true;
                break;
            }

            --bytePattern_index;
            currentByteMatchPatternIndex = bytePattern.bytePattern[bytePattern_index].first;
        }
        return matchResult;
    }

    uint32_t PatternScanner::DetermineBadByteHeuristicOffset(const MatchResult& matchResult) const {
        uint32_t badByteHeuristicOffset;

        if(matchResult.FoundFullMatch)
            badByteHeuristicOffset = bytePattern.GetSize();
        else
            badByteHeuristicOffset = matchResult.MismatchIndex;

        return badByteHeuristicOffset;
    }

    bool PatternScanner::DoesPatternReachPastEndOffset(uintptr_t currentMemoryOffset) const {
        bool doesPatternReachPastEndAddress = currentMemoryOffset + bytePattern.GetSize() - 1 > memory_maximumByteOffset;
        return doesPatternReachPastEndAddress;
    }

    bool PatternScanner::DoesOffsetReachPastScanEndAddress(uintptr_t memoryOffset) const {
        bool doesOffsetReachPastScanEndAddress = memoryOffset > memory_maximumByteOffset;

        return doesOffsetReachPastScanEndAddress;
    }
}
