#include <utility>

#include "gtest/gtest.h"
#include "Exceptions/WinAPIException.h"
#include "Memory/MemoryManager.h"
#include "Memory/Internal/MemoryManagerInternal.h"
#include "Memory/Scan/MemoryScanner.h"
#include "Memory/Scan/PatternScanner.h"
#include "Memory/Scan/MemoryScannerInternal.h"

namespace MemoryCommando::Memory {
    using namespace testing;

    static BYTE staticByteArray[10] = {0xda, 0xd9, 0x48, 0x53, 0x9b, 0xcd, 0x21, 0x00, 0x8d, 0x8f};

    class MemoryScannerInternalF : public Test {
    public:
        std::shared_ptr<MemoryManagerInternal> MemoryManager{std::make_shared<MemoryManagerInternal>()};
        MemoryScannerInternal MemoryScanner{MemoryManager};
        std::vector<BYTE> ByteText{};
    protected:
        void TearDown() override {
            for(auto& byte: ByteText) {
                byte = 0;
            }
        }

        void InitializeVector(std::vector<BYTE>& byteText) {
            byteText.emplace_back(0x43);
            byteText.emplace_back(0x5C);
            byteText.emplace_back(0xCA);
            byteText.emplace_back(0xF0);
            byteText.emplace_back(0x18);
            byteText.emplace_back(0x33);
            byteText.emplace_back(0x17);
            byteText.emplace_back(0xF0);
            byteText.emplace_back(0x31);
            byteText.emplace_back(0xBC);
            byteText.emplace_back(0xC4);
            byteText.emplace_back(0xFC);
            byteText.emplace_back(0x00);
        }

        void InitializeByteVectorFromIntegerVector(std::vector<BYTE>& byteVector, std::vector<int>& integerVector) {
            for(const auto& currentInteger : integerVector) {
                byteVector.emplace_back(static_cast<BYTE>(currentInteger));
            }
        }

        std::wstring StringToWideString(const std::string& str)
        {
            int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
            std::wstring wstrTo(size_needed, 0);
            MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
            return wstrTo;
        }

        template<class Classname>
        void NullOutVector(std::vector<Classname>& vector) {
            for(auto& currentMember : vector) {
                currentMember = 0;
            }
        }
    };

    TEST_F(MemoryScannerInternalF, GivenStartAndEndAddressAndBytePattern_ScansCorrectly) {
        ByteText = {0x43, 0x5C, 0xCA, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC};
        BytePattern bytePattern("CA F0 18 33 17 F0 31 BC C4 FC");

        uintptr_t scanStartAddress = (uintptr_t)&ByteText.front();
        uintptr_t scanEndAddress = (uintptr_t)&ByteText.back();
        auto scanResults = MemoryScanner.ScanVirtualMemory(scanStartAddress, scanEndAddress, bytePattern);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults[0], (uintptr_t)&ByteText[2]);
    }

    TEST_F(MemoryScannerInternalF, GivenStartAndEndAddressAndByteSequence_ScansCorrectly) {
        ByteText = {0x43, 0x5C, 0xCA, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC};
        ByteSequence byteSequence{0xCA, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC};

        intptr_t scanStartAddress = (uintptr_t)&ByteText.front();
        uintptr_t scanEndAddress = (uintptr_t)&ByteText.back();

        auto scanResults = MemoryScanner.ScanVirtualMemory(scanStartAddress, scanEndAddress, byteSequence);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults[0], (uintptr_t)&ByteText[2]);
    }

    TEST_F(MemoryScannerInternalF, GivenStartAndEndAddressAndStringPattern_ScansCorrectly) {
        ByteText = {0x00, 0x01, 0x05, 0x10, 0xFC};
        ScanResults scanResults = MemoryScanner.ScanVirtualMemory((uintptr_t)&ByteText.front(), (uintptr_t)&ByteText.back(), std::string("05 10 FC"));

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults[0], (uintptr_t)&ByteText[2]);
    }

    TEST_F(MemoryScannerInternalF, GivenStartAndEndAddressAndObject_ScansCorrectly) {
        ByteText = {0x43, 0x5C, 0xCA, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC, 0x62, 0xA0, 0x35, 0x34, 0x81};

        uintptr_t scanStartAddress = (uintptr_t)&ByteText.front();
        uintptr_t scanEndAddress = (uintptr_t)&ByteText.back();

        UINT64 integerObject{};
        const auto bytePattern = std::vector<BYTE>{0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0};
        std::memcpy(&integerObject, &bytePattern[0], bytePattern.size());

        auto scanResults = MemoryScanner.ScanVirtualMemory(scanStartAddress, scanEndAddress, integerObject);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)&ByteText.front() + 3);
    }
    TEST_F(MemoryScannerInternalF, GivenStartAddressAndBytePattern_ScansCorrectly) {
        InitializeVector(ByteText);
        BytePattern bytePattern{"CA F0 18 33 17 F0 31 BC C4 FC"};

        auto scanResults = MemoryScanner.ScanVirtualMemory((uintptr_t)&ByteText.front(), bytePattern);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)&ByteText.front() + 2);
    }

    TEST_F(MemoryScannerInternalF, GivenStartAndByteSequence_ScansCorrectly) {
        InitializeVector(ByteText);
        ByteSequence byteSequence{0xCA, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC};

        intptr_t scanStartAddress = (uintptr_t)&ByteText.front();

        auto scanResults = MemoryScanner.ScanVirtualMemory(scanStartAddress, byteSequence);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults[0], (uintptr_t)&ByteText[2]);
    }

    TEST_F(MemoryScannerInternalF, GivenStartAddressAndStringPattern_ScansCorrectly) {
        InitializeVector(ByteText);
        const std::string pattern = "CA F0 18 33 17 F0 31 BC C4 FC";

        auto scanResults = MemoryScanner.ScanVirtualMemory((uintptr_t)&ByteText.front(), pattern);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)&ByteText.front() + 2);
    }

    TEST_F(MemoryScannerInternalF, GivenStartAddressAndObject_ScansCorrectly) {
        std::vector<int> integerVector = {0x43, 0x5C, 0xCA, 0xF0, 0x18, 0x33, 0xF0, 0x31, 0xBC, 0xC4, 0xFC, 0x00};
        InitializeByteVectorFromIntegerVector(ByteText, integerVector);
        std::vector<BYTE> byteArray{};
        integerVector = {0xCA, 0xF0, 0x18, 0x33, 0xF0, 0x31, 0xBC, 0xC4};
        InitializeByteVectorFromIntegerVector(byteArray, integerVector);

        uintptr_t scanStartAddress = (uintptr_t)&ByteText.front();

        UINT64& integerObject = *reinterpret_cast<UINT64*>(&byteArray[0]);

        auto scanResults = MemoryScanner.ScanVirtualMemory(scanStartAddress, integerObject);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)&ByteText.front() + 2);
    }

    TEST_F(MemoryScannerInternalF, GivenOnlyBytePattern_ScansCorrectly) {
        InitializeVector(ByteText);

        BytePattern bytePattern{"43 5C CA F0 18 33 17 F0 31 BC C4 FC 00"};

        auto scanResults = MemoryScanner.ScanVirtualMemory(bytePattern);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)&ByteText.front());
    }

    TEST_F(MemoryScannerInternalF, GivenOnlyByteSequence_Finds3Results) {
        InitializeVector(ByteText);

        ByteSequence byteSequence{0xCA, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC};

        auto scanResults = MemoryScanner.ScanVirtualMemory(byteSequence);

        ASSERT_EQ(scanResults.size(), 1);
    }
    TEST_F(MemoryScannerInternalF, GivenOnlyStringPattern_ScansCorrectly) {
        InitializeVector(ByteText);

        const std::string pattern = "43 5C CA F0 18 33 17 F0 31 BC C4 FC 00";

        auto scanResults = MemoryScanner.ScanVirtualMemory(pattern);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)&ByteText.front());
    }

    TEST_F(MemoryScannerInternalF, GivenOnlyObject_ScansCorrectly) {
        std::vector<int> integerVector{0x43, 0x5C, 0xCA, 0xF0, 0x18, 0x33, 0xF0, 0x31, 0xBC, 0xC4, 0xFC, 0x00};
        InitializeByteVectorFromIntegerVector(ByteText, integerVector);
        std::vector<BYTE> byteArray{};

        integerVector = {0xCA, 0xF0, 0x18, 0x33, 0xF0, 0x31, 0xBC, 0xC4};
        InitializeByteVectorFromIntegerVector(byteArray, integerVector);

        UINT64& integerObject = *reinterpret_cast<UINT64*>(&byteArray.front());

        auto scanResults = MemoryScanner.ScanVirtualMemory(integerObject);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)&ByteText.front() + 2);
    }
    TEST_F(MemoryScannerInternalF, GivenModuleNameAndBytePattern_ScansCorrectly) {
        auto* byteArr = staticByteArray;

        BytePattern bytePattern{"da d9 48 53 9b cd 21 00 8d 8f"};

        std::string mainModuleName(PROJECT_NAME".exe");
        auto scanResults = MemoryScanner.ScanVirtualMemory(StringToWideString(mainModuleName), bytePattern);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)byteArr);
    }

    TEST_F(MemoryScannerInternalF, GivenModuleNameAndByteSequence_ScansCorrectly) {
        auto* byteArr = staticByteArray;
        ByteSequence byteSequence{0xda, 0xd9, 0x48, 0x53, 0x9b, 0xcd, 0x21, 0x00, 0x8d, 0x8f};

        std::string mainModuleName(PROJECT_NAME".exe");

        auto scanResults = MemoryScanner.ScanVirtualMemory(StringToWideString(mainModuleName), byteSequence);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)byteArr);
    }

    TEST_F(MemoryScannerInternalF, GivenModuleNameAndStringPattern_ScansCorrectly) {
        auto* byteArr = staticByteArray;
        std::string pattern = "da d9 48 53 9b cd 21 00 8d 8f";

        std::string projectName(PROJECT_NAME".exe");
        auto scanResults = MemoryScanner.ScanVirtualMemory(StringToWideString(projectName), pattern);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)byteArr);
    }

    TEST_F(MemoryScannerInternalF, GivenModuleNameAndObject_ScansCorrectly) {
        auto* byteArr = staticByteArray;

        UINT64 integerObject{};
        std::vector<BYTE> bytePattern{0xda, 0xd9, 0x48, 0x53, 0x9b, 0xcd, 0x21, 0x00};
        std::memcpy(&integerObject, &bytePattern[0], bytePattern.size());

        std::string projectName(PROJECT_NAME".exe");
        auto scanResults = MemoryScanner.ScanVirtualMemory(StringToWideString(projectName), integerObject);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)byteArr);
    }

    TEST_F(MemoryScannerInternalF, GivenSequenceOfModuleNamesAndByteSequence_ScansCorrectly) {
        std::vector<MODULEENTRY32W> modules = HelperMethods::GetModules(GetCurrentProcessId());

        auto* byteArr = staticByteArray;
        ByteSequence byteSequence{0xda, 0xd9, 0x48, 0x53, 0x9b, 0xcd, 0x21, 0x00, 0x8d, 0x8f};

        std::vector<std::wstring> moduleNames;
        moduleNames.reserve(modules.size());
        for(auto currentModule : modules) {
            moduleNames.emplace_back(currentModule.szModule);
        }

        auto scanResults = MemoryScanner.ScanVirtualMemory(moduleNames, byteSequence);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)byteArr);
    }
    TEST_F(MemoryScannerInternalF, GivenSequenceOfModuleNamesAndBytePattern_ScansCorrectly) {
        std::vector<MODULEENTRY32W> modules = HelperMethods::GetModules(GetCurrentProcessId());

        auto* byteArr = staticByteArray;
        BytePattern bytePattern{"da d9 48 53 9b cd 21 00 8d 8f"};

        std::vector<std::wstring> moduleNames;
        moduleNames.reserve(modules.size());
        for(auto currentModule : modules) {
            moduleNames.emplace_back(currentModule.szModule);
        }

        auto scanResults = MemoryScanner.ScanVirtualMemory(moduleNames, bytePattern);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)byteArr);
    }
    TEST_F(MemoryScannerInternalF, GivenSequenceOfModuleNamesAndStringPattern_ScansCorrectly) {
        std::vector<MODULEENTRY32W> modules = HelperMethods::GetModules(GetCurrentProcessId());

        auto* byteArr = staticByteArray;
        std::string pattern = "da d9 48 53 9b cd 21 00 8d 8f";

        std::vector<std::wstring> moduleNames;
        moduleNames.reserve(modules.size());
        for(auto currentModule : modules) {
            moduleNames.emplace_back(currentModule.szModule);
        }

        auto scanResults = MemoryScanner.ScanVirtualMemory(moduleNames, pattern);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)byteArr);
    }

    TEST_F(MemoryScannerInternalF, GivenSequenceOfModuleNamesAndObject_ScansCorrectly) {
        std::vector<MODULEENTRY32W> modules = HelperMethods::GetModules(GetCurrentProcessId());

        auto* byteArr = staticByteArray;

        UINT64 integerObject{};
        std::vector<BYTE> bytePattern{0xda, 0xd9, 0x48, 0x53, 0x9b, 0xcd, 0x21, 0x00};
        std::memcpy(&integerObject, &bytePattern[0], bytePattern.size());

        std::vector<std::wstring> moduleNames;
        moduleNames.reserve(modules.size());
        for(auto currentModule : modules) {
            moduleNames.emplace_back(currentModule.szModule);
        }

        auto scanResults = MemoryScanner.ScanVirtualMemory(moduleNames, integerObject);

        ASSERT_EQ(scanResults.size(), 1);
        ASSERT_EQ(scanResults.front(), (uintptr_t)byteArr);
    }

    class MemoryScannerInternalTwoMemoryRegionsF : public MemoryScannerInternalF {
    public:
        size_t AllocationSize{};
        uintptr_t AllocationAddress{};
        uintptr_t EndAddress{};
        uintptr_t ThirdMemoryRegionAddress{};
    protected:
        void TearDown() override {
            MemoryManager->FreeVirtualMemory(AllocationAddress);
        }
    };
    TEST_F(MemoryScannerInternalTwoMemoryRegionsF, ScansTwoMemoryRegionsWithUnreachableMemoryBetweenThem) {
        ByteText = {0x00, 0x01, 0x05, 0x10, 0xFC};
        AllocationSize = 0x3000;

        AllocationAddress = reinterpret_cast<uintptr_t>(VirtualAlloc(nullptr, AllocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
        EndAddress = AllocationAddress + AllocationSize;
        if(!AllocationAddress)
            FAIL("Couldn't allocate memory with VirtualAlloc");

        DWORD flOldProtect;
        LPVOID protectAddress = reinterpret_cast<LPVOID>(AllocationAddress + 0x1000);
        VirtualProtect(protectAddress, 0x1000, PAGE_NOACCESS, &flOldProtect);

        ThirdMemoryRegionAddress = AllocationAddress + 0x2000;

        std::memcpy((LPVOID)AllocationAddress, &ByteText[0], ByteText.size());
        std::memcpy((LPVOID)ThirdMemoryRegionAddress, &ByteText[0], ByteText.size());

        auto results = MemoryScanner.ScanVirtualMemory(AllocationAddress, EndAddress, std::string("05 10 FC"));
        ASSERT_EQ(results.size(), 2);
        ASSERT_EQ(results[0], AllocationAddress + 2);
        ASSERT_EQ(results[1], ThirdMemoryRegionAddress + 2);
    }
    TEST_F(MemoryScannerInternalF, GetsReadableMemoryRegions) {
        size_t allocationSize = 0x3000;
        uintptr_t allocationAddress = reinterpret_cast<uintptr_t>(VirtualAlloc(nullptr, allocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
        uintptr_t endAddress = allocationAddress + allocationSize;
        if(!allocationAddress)
            FAIL("Couldn't allocate memory with VirtualAlloc");

        DWORD flOldProtect;
        LPVOID protectAddress = reinterpret_cast<LPVOID>(allocationAddress + 0x1000);
        VirtualProtect(protectAddress, 0x1000, PAGE_NOACCESS, &flOldProtect);

        uintptr_t thirdMemoryRegionAddress = allocationAddress + 0x2000;

        auto memoryRegions = MemoryScanner.GetReadableMemoryRegions(allocationAddress, endAddress);
        EXPECT_EQ(memoryRegions.size(), 2);
    }
    TEST_F(MemoryScannerInternalF, IsMemoryRegionReadable) {
        size_t allocationSize = 0x5000;
        uintptr_t allocationAddress = reinterpret_cast<uintptr_t>(VirtualAlloc(nullptr, allocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
        bool isMemoryRegionReadable = MemoryScanner.IsMemoryRegionReadable(allocationAddress);

        ASSERT_EQ(isMemoryRegionReadable, true);
        BYTE* pByte = reinterpret_cast<BYTE*>(allocationAddress);
        pByte[0] = 0xFA;

        DWORD oldProtect;
        bool succeeded = VirtualProtect((LPVOID)allocationAddress, allocationSize, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &oldProtect);
        if(!succeeded)
            FAIL("VirtualProtect failed with error code: " + GetLastError());

        isMemoryRegionReadable = MemoryScanner.IsMemoryRegionReadable(allocationAddress);
        ASSERT_EQ(isMemoryRegionReadable, false);
        
        VirtualProtect((LPVOID)allocationAddress, allocationSize, PAGE_NOACCESS, &oldProtect);
        isMemoryRegionReadable = MemoryScanner.IsMemoryRegionReadable(allocationAddress);
        ASSERT_EQ(isMemoryRegionReadable, false);
    }
    TEST_F(MemoryScannerInternalF, IsMemoryRegionUsed) {
        size_t allocationSize = 0x5000;
        uintptr_t allocationAddress = reinterpret_cast<uintptr_t>(VirtualAlloc(nullptr, allocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
        bool isMemoryRegionUsed = MemoryScanner.IsMemoryRegionUsed(allocationAddress);
        ASSERT_EQ(isMemoryRegionUsed, true);

        bool decommitted = VirtualFree((LPVOID)allocationAddress, 0, MEM_DECOMMIT);
        isMemoryRegionUsed = MemoryScanner.IsMemoryRegionUsed(allocationAddress);
        ASSERT_EQ(isMemoryRegionUsed, false);
    }
}