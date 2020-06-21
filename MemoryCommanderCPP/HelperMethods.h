#pragma once
#include <codecvt>
#include <locale>
#include <string>

namespace MemoryCommanderCPP::Shared {
    class HelperMethods {
    public:
        static std::wstring ConvertStringToWString(const std::string& narrowString);
        static std::string ConvertWideStringToString(const std::wstring& wideString);
    private:

    };
}
