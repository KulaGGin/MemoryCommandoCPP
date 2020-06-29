#pragma once
#include <CppUnitTest.h>


namespace MemoryCommandoTests {
    TEST_CLASS(MemoryManagerExternalTests) {
    public:
        MemoryManagerExternalTests();

        TEST_METHOD(ConstructorProcessId);
        TEST_METHOD(ConstructorProcessNameWide);
        TEST_METHOD(ConstructorNonExistentName);
        TEST_METHOD(GetModulesHandles);
    private:
        DWORD _currentProcessId;
        HANDLE _currentProcessHandle;
        std::wstring _currentProcessName;

        std::wstring GetCurrentProcessName();
    };
}
