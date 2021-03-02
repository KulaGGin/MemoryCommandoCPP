// ReSharper disable CppMemberFunctionMayBeStatic
// ReSharper disable CppMemberFunctionMayBeConst
#include "HelperMethodsTests.h"

#include <Psapi.h>

#include "Exceptions/GetProcessIdException.h"
#include "Exceptions/OpenProcessException.h"
#include "HelperMethods.h"
#include "Exceptions/WinAPIException.h"

namespace MemoryCommandoTests {
    using namespace MemoryCommando;
    using namespace Exceptions;
    using namespace  Microsoft::VisualStudio::CppUnitTestFramework;

    HelperMethodsTests::HelperMethodsTests():
        _currentProcessId(GetCurrentProcessId()),
        _currentProcessHandle(OpenProcess(PROCESS_ALL_ACCESS, 0, _currentProcessId)),
        _currentProcessName(GetCurrentProcessName()) {
    }

    void HelperMethodsTests::GetSystemInfo() {
        auto systemInfo = HelperMethods::GetSystemInfo();
    }

    void HelperMethodsTests::GetRunningProcesses() {
        try {
            const auto runningProcesses = HelperMethods::GetRunningProcesses();

            if(runningProcesses.empty())
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
           Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetProcessByProcessId() {
        try {
            const auto process = HelperMethods::GetProcess(_currentProcessId);
            if(process.th32ProcessID != _currentProcessId)
                Assert::IsTrue(false);
        }
        catch(std::runtime_error&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetProcessByProcessHandle() {
        try {
            const auto process = HelperMethods::GetProcess(_currentProcessHandle);
            if(process.th32ProcessID != _currentProcessId)
                Assert::IsTrue(false);
        }
        catch(std::runtime_error&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetProcessByProcessName() {
        try {
            const auto process = HelperMethods::GetProcess(_currentProcessName);
            if(process.th32ProcessID != _currentProcessId)
                Assert::IsTrue(false);
        }
        catch(std::runtime_error&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetProcessIdByProcessHandle() {
        try {
            const auto processId = HelperMethods::GetProcessId(_currentProcessHandle);
            if(_currentProcessId != processId)
                Assert::IsTrue(false);
        }
        catch(GetProcessIdException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetProcessIdByProcessName() {
        try {
            const auto processId = HelperMethods::GetProcessId(_currentProcessName);
            if(_currentProcessId != processId)
                Assert::IsTrue(false);
        }
        catch(std::runtime_error&) {
            Assert::IsTrue(false);
        }

    }

    void HelperMethodsTests::GetProcessHandleByProcessId() {
        try {
            const auto processHandle = HelperMethods::GetProcessHandle(_currentProcessId);
            if(!processHandle)
                Assert::IsTrue(false);
        }
        catch(OpenProcessException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetProcessHandleByProcessName() {
        try {
            const auto processHandle = HelperMethods::GetProcessHandle(_currentProcessName);
            if(!processHandle)
                Assert::IsTrue(false);
        }
        catch(std::runtime_error&) {
            Assert::IsTrue(false);
        }
        catch(OpenProcessException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetProcessNameByProcessHandle() {
        try {
            const auto processName = HelperMethods::GetProcessName(_currentProcessHandle);
            if(processName != _currentProcessName)
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetProcessNameByProcessId() {
        try {
            const auto processName = HelperMethods::GetProcessName(_currentProcessId);
            if(processName != _currentProcessName)
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetModulesByProcessId() {
        try {
            auto modules = HelperMethods::GetModules(_currentProcessId);
            if(modules.empty())
                Assert::IsTrue(false);
            if(modules[0].th32ProcessID != _currentProcessId)
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetModulesByProcessHandle() {
        try {
            auto modules = HelperMethods::GetModules(_currentProcessHandle);
            if(modules.empty())
                Assert::IsTrue(false);
            if(modules[0].th32ProcessID != _currentProcessId)
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetModulesByProcessName() {
        try {
            auto modules = HelperMethods::GetModules(_currentProcessName);
            if(modules.empty())
                Assert::IsTrue(false);
            if(modules[0].th32ProcessID != _currentProcessId)
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetModuleByProcessId() {
        try {
            auto modules = HelperMethods::GetModules(_currentProcessId);
            auto moduleInstance = HelperMethods::GetModule(_currentProcessName, _currentProcessId);
            if(moduleInstance.szModule != _currentProcessName)
                Assert::IsTrue(false);
            if(moduleInstance.modBaseAddr != modules[0].modBaseAddr)
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetModuleByProcessHandle() {
        try {
            auto modules = HelperMethods::GetModules(_currentProcessId);
            auto moduleInstance = HelperMethods::GetModule(_currentProcessName, _currentProcessHandle);
            if(moduleInstance.szModule != _currentProcessName)
                Assert::IsTrue(false);
            if(moduleInstance.modBaseAddr != modules[0].modBaseAddr)
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetModuleByProcessName() {
        try {
            auto modules = HelperMethods::GetModules(_currentProcessId);
            auto moduleInstance = HelperMethods::GetModule(_currentProcessName, _currentProcessName);
            if(moduleInstance.szModule != _currentProcessName)
                Assert::IsTrue(false);
            if(moduleInstance.modBaseAddr != modules[0].modBaseAddr)
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetModuleBaseAddressByProcessId() {
        try {
            auto modules = HelperMethods::GetModules(_currentProcessId);
            const auto moduleBaseAddress = HelperMethods::GetModuleBaseAddress(_currentProcessName, _currentProcessId);

            if(moduleBaseAddress != uintptr_t(modules[0].modBaseAddr))
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetModuleBaseAddressByProcessHandle() {
        try {
            auto modules = HelperMethods::GetModules(_currentProcessId);
            const auto moduleBaseAddress = HelperMethods::GetModuleBaseAddress(_currentProcessName, _currentProcessHandle);

            if(moduleBaseAddress != uintptr_t(modules[0].modBaseAddr))
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetModuleBaseAddressByProcessName() {
        try {
            auto modules = HelperMethods::GetModules(_currentProcessId);
            const auto moduleBaseAddress = HelperMethods::GetModuleBaseAddress(_currentProcessName, _currentProcessName);

            if(moduleBaseAddress != uintptr_t(modules[0].modBaseAddr))
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetModuleSizeByProcessId() {
        try {
            auto modules = HelperMethods::GetModules(_currentProcessId);
            const auto moduleSize = HelperMethods::GetModuleSize(_currentProcessName, _currentProcessId);

            if(moduleSize != modules[0].modBaseSize)
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetModuleSizeByProcessHandle() {
        try {
            auto modules = HelperMethods::GetModules(_currentProcessId);
            const auto moduleSize = HelperMethods::GetModuleSize(_currentProcessName, _currentProcessHandle);

            if(moduleSize != modules[0].modBaseSize)
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::GetModuleSizeByProcessName() {
        try {
            auto modules = HelperMethods::GetModules(_currentProcessId);
            const auto moduleSize = HelperMethods::GetModuleSize(_currentProcessName, _currentProcessName);

            if(moduleSize != modules[0].modBaseSize)
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::ConvertObjectToBytes() {
        const unsigned long long testNumber = 0x0123456789ABCDEF;
        const std::vector<BYTE> testNumberBytes = { 0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01 };

        const auto numberBytesConverted = HelperMethods::ConvertObjectToBytes(testNumber);

        if(numberBytesConverted.size() != testNumberBytes.size())
            Assert::IsTrue(false);

        for(size_t index = 0; index < testNumberBytes.size(); ++index) {
            if(testNumberBytes[index] != numberBytesConverted[index])
                Assert::IsTrue(false);
        }
    }

    void HelperMethodsTests::ConvertBytesToObject() {
        const std::vector<BYTE> testNumberBytes = { 0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01 };
        const unsigned long long testNumber = 0x0123456789ABCDEF;

        const auto numberConverted = HelperMethods::ConvertBytesToObject<unsigned long long>(testNumberBytes);
        if(numberConverted != testNumber)
            Assert::IsTrue(false);
    }

    std::wstring HelperMethodsTests::GetCurrentProcessName() {
        const auto fileNamePointer = std::make_unique<CHAR[]>(MAX_PATH);

        GetModuleBaseNameA(_currentProcessHandle, nullptr, fileNamePointer.get(), MAX_PATH * sizeof(CHAR));
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> stringToWStringConverter;
        std::wstring processName = stringToWStringConverter.from_bytes(fileNamePointer.get());

        return processName;
    }
}