#include "HelperMethods.h"

#include <string>
#include <windows.h>
#include <stdio.h>
#include <wchar.h>

namespace MemoryCommanderCPP::Shared {
    // Convert an UTF8 string to a wide Unicode String
    std::wstring HelperMethods::ConvertStringToWString(const std::string& narrowString)
    {
        if (narrowString.empty()) return std::wstring();
        const int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, &narrowString[0], static_cast<int>(narrowString.size()), nullptr, 0);
        std::wstring wideStringTo(sizeNeeded, 0);
        MultiByteToWideChar(CP_UTF8, 0, &narrowString[0], static_cast<int>(narrowString.size()), &wideStringTo[0], sizeNeeded);
        return wideStringTo;
    }

    // Convert a wide Unicode string to an UTF8 string
    std::string HelperMethods::ConvertWideStringToString(const std::wstring& wideString) {
        if(wideString.empty()) return std::string();
        const int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wideString[0], static_cast<int>(wideString.size()), nullptr, 0,
                                                    nullptr, nullptr);
        std::string strTo(sizeNeeded, 0);
        WideCharToMultiByte(CP_UTF8, 0, &wideString[0], static_cast<int>(wideString.size()), &strTo[0], sizeNeeded,
                            nullptr, nullptr);
        return strTo;
    }
}
