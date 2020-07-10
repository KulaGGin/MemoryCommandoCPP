#pragma once

#include <windows.h>
#include <string>
#include <vector>

namespace MemoryCommando::Memory {
    class ScanHelperMethods {
    public:
        static std::vector<std::pair<size_t, BYTE>> GetIndexedPattern(const std::string& pattern);
        static std::vector<std::pair<size_t, BYTE>> GetIndexedPattern(const std::vector<std::string>& pattern);
        static std::vector<std::pair<size_t, BYTE>> GetIndexedPattern(const std::vector<BYTE>& pattern);
    };
}

