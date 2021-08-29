#pragma once
#include <cstdint>
#include <vector>

namespace MemoryCommando::Memory {
    using ScanResults = std::vector<uintptr_t>;
    using StringPattern = std::string;
    using IndexedByte = std::pair<uint32_t, int>;
    using IndexedBytePattern = std::vector<IndexedByte>;
    using ByteSequence = std::vector<int>;
    using PatternMembersSequence = std::vector<std::string>;
}
