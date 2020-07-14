#pragma once
#include <windows.h>
#include <CppUnitTest.h>

#include "MemoryCommandoTestsHelper.h"


namespace MemoryCommandoTests {
    TEST_CLASS(HelperMethodsTests) {
    public:
        HelperMethodsTests();

        TEST_METHOD(GetSystemInfo);
        TEST_METHOD(GetRunningProcesses);

        TEST_METHOD(GetProcessByProcessId);
        TEST_METHOD(GetProcessByProcessHandle);
        TEST_METHOD(GetProcessByProcessName);

        TEST_METHOD(GetProcessIdByProcessHandle);
        TEST_METHOD(GetProcessIdByProcessName);

        TEST_METHOD(GetProcessHandleByProcessId);
        TEST_METHOD(GetProcessHandleByProcessName);

        TEST_METHOD(GetProcessNameByProcessHandle);
        TEST_METHOD(GetProcessNameByProcessId);

        TEST_METHOD(GetModulesByProcessId);
        TEST_METHOD(GetModulesByProcessHandle);
        TEST_METHOD(GetModulesByProcessName);

        TEST_METHOD(GetModuleByProcessId);
        TEST_METHOD(GetModuleByProcessHandle);
        TEST_METHOD(GetModuleByProcessName);

        TEST_METHOD(GetModuleBaseAddressByProcessId);
        TEST_METHOD(GetModuleBaseAddressByProcessHandle);
        TEST_METHOD(GetModuleBaseAddressByProcessName);

        TEST_METHOD(GetModuleSizeByProcessId);
        TEST_METHOD(GetModuleSizeByProcessHandle);
        TEST_METHOD(GetModuleSizeByProcessName);

        TEST_METHOD(ConvertObjectToBytes);
        TEST_METHOD(ConvertBytesToObject);
    private:
        DWORD        _currentProcessId;
        HANDLE       _currentProcessHandle;
        std::wstring _currentProcessName;

        std::wstring GetCurrentProcessName();

        MemoryCommandoTestsHelper _memoryCommandoTestsHelper;
    };
}
