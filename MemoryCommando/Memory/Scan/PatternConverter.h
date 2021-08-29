#pragma once

#include <activation.h>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <iostream>

#include "ScanTypeDefinitions.h"

namespace MemoryCommando::Memory {
    class PatternConverter {
    public:
        IndexedBytePattern StringToIndexedPattern(const StringPattern& stringPattern);

        PatternMembersSequence SplitPatternToPatternMembers(const StringPattern& stringPattern);

        IndexedBytePattern ConvertPatternMembersToIndexedBytePattern(const PatternMembersSequence& patternMembers) const;
        IndexedBytePattern ConvertByteSequenceToIndexedBytePattern(const ByteSequence& byteSequence) const;
    };
}
