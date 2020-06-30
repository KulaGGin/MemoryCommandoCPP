#pragma once
#include <map>
#include <windows.h>
#include <string>
#include <vector>

namespace MemoryCommandoTests {
    class BytePatternScannerTests;
}

namespace MemoryCommando::Scan {
    class BytePatternScanner {
    public:
        std::vector<size_t> Scan(std::vector<BYTE> byteSequence, std::string pattern);
    private:
        size_t GetLastWildcardIndex(const std::vector<std::string>& patternStringSequence);
        std::vector<std::pair<size_t, BYTE>> GetFilteredPattern(std::vector<std::string> pattern);
        std::map<int, size_t> GenerateBadByteTable(const std::vector<std::string>& patternStringSequence);
        std::vector<size_t> GetPatternIndexes(const std::vector<BYTE>& byteSequence, std::string pattern);

        friend class MemoryCommandoTests::BytePatternScannerTests;
    };
}
