#pragma once

#include <activation.h>
#include <string>
#include <vector>

#include "PatternConverter.h"

namespace MemoryCommando::Memory {
    class BytePattern {
    public:
        BytePattern() = default;
        explicit BytePattern(const std::string& pattern);
        explicit BytePattern(const ByteSequence& byteSequence);
        uint32_t GetSize() const;
        IndexedBytePattern GetIndexedPattern() const;

        IndexedByte& operator[](int index);

        IndexedBytePattern indexedPattern{};
    private:
        PatternConverter patternConverter{};

        uint32_t size{};
    };
}

