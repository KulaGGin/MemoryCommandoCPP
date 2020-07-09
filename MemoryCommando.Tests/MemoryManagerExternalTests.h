#pragma once
#include <CppUnitTest.h>
#include <minwindef.h>
#include <strsafe.h>


#include "../MemoryCommando/Memory/External/MemoryManagerExternal.h"

namespace MemoryCommandoTests {
    using namespace MemoryCommando::Memory::External;
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
        std::string _currentProcessName;

        MemoryManagerExternal _memoryManagerExternal;

        std::string GetCurrentProcessName();
    };
}
