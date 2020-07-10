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
        std::vector<size_t> Scan(const std::vector<BYTE>& byteSequence, const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const;
        std::vector<size_t> Scan(const std::vector<BYTE>& byteSequence, const std::vector<BYTE>& bytePattern) const;
        std::vector<size_t> Scan(const std::vector<BYTE>& byteSequence, const std::string& pattern) const;
    private:
        const unsigned _byteQuantity = MAXBYTE + 1;

        std::map<int, size_t> GenerateBadByteTable(const std::vector<std::pair<size_t, BYTE>>& indexedPattern) const;

        friend class MemoryCommandoTests::BytePatternScannerTests;
    };
}
