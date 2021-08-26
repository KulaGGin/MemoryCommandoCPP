#pragma once

#include <activation.h>
#include <string>
#include <vector>

#include "PatternConverter.h"

namespace MemoryCommando::Memory {
    class BytePattern {
    public:
        BytePattern() = default;
        BytePattern(const std::string& pattern);
        uint32_t GetSize() const;
        std::vector<std::pair<uint32_t, BYTE>> GetPattern() const;

        std::pair<uint32_t, BYTE>& operator[](int index);

        std::vector<std::pair<uint32_t, BYTE>> bytePattern{};
    private:
        PatternConverter patternConverter{};

        uint32_t size{};
    };
}

