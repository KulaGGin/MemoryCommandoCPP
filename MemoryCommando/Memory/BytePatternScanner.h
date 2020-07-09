#pragma once
#include <map>
#include <windows.h>
#include <string>
#include <vector>

namespace MemoryCommandoTests {
    class BytePatternScannerTests;
}

namespace MemoryCommando::Memory {
    class BytePatternScanner {
    public:
        std::vector<size_t> Scan(std::vector<BYTE> byteSequence, std::string pattern) const;
    private:
        size_t GetLastWildcardIndex(const std::vector<std::string>& patternStringSequence) const;
        std::vector<std::pair<size_t, BYTE>> GetFilteredPattern(std::vector<std::string> pattern) const;
        std::map<int, size_t> GenerateBadByteTable(const std::vector<std::string>& patternStringSequence) const;
        std::vector<size_t> GetPatternIndexes(const std::vector<BYTE>& byteSequence, std::string pattern) const;

        friend class MemoryCommandoTests::BytePatternScannerTests;
    };
}
