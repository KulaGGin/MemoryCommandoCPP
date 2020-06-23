#pragma once
#include <CppUnitTest.h>

namespace MemoryCommandoTests {
    using namespace MemoryCommando;
    TEST_CLASS(MemoryManagerExternalTests) {
    public:
        MemoryManagerExternalTests();

        TEST_METHOD(ConstructorProcessId);
        TEST_METHOD(ConstructorProcessNameWide);
        TEST_METHOD(ConstructorProcessNameNarrow);
        TEST_METHOD(ConstructorNonExistentName);
        TEST_METHOD(GetModulesHandles);
    private:
        DWORD _currentProcessId;
        HANDLE _currentProcessHandle;
        std::string _currentProcessName;

        std::string GetCurrentProcessName();
    };
}
