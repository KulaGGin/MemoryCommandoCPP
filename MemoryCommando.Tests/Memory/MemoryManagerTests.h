#pragma once
#include <CppUnitTest.h>
#include <CppUnitTestAssert.h>
#include <windows.h>

#include "MemoryCommando/Memory/MemoryManager.h"

namespace MemoryCommandoTests {
    using namespace MemoryCommando;
    using namespace Memory;
    TEST_CLASS(MemoryManagerTests) {
    public:
        TEST_METHOD(GetModules);
        MemoryManagerTests();
    private:
        const DWORD        _currentProcessId;
        const HANDLE       _currentProcessHandle;
        const std::wstring _currentProcessName;

        std::shared_ptr<MemoryManager> _memoryManager;

        std::wstring GetCurrentProcessName();
        std::shared_ptr<MemoryManager> GetMemoryManager(DWORD processId);
    };
}
