#pragma once
#include <codecvt>
#include <locale>
#include <string>

namespace MemoryCommanderCpp::Shared {
    class HelperMethods {
    public:
        static std::wstring ConvertStringToWString(const std::string& narrowString);
        static std::string ConvertWideStringToString(const std::wstring& wideString);
    private:

    };
}
