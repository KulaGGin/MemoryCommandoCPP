#pragma once

#include <CppUnitTest.h>

#include "MemoryCommando/Memory/Internal/MemoryManagerInternal.h"

namespace MemoryCommandoTests {
    using namespace MemoryCommando;
    using namespace Memory::Internal;
    using namespace Microsoft::VisualStudio::CppUnitTestFramework;
    TEST_CLASS(MemoryManagerInternalTests) {
    public:
        MemoryManagerInternalTests();
        TEST_METHOD(Constructor);
        TEST_METHOD(GetProcessId);
        TEST_METHOD(GetProcessHandle);
        TEST_METHOD(AllocateVirtualMemory);
        TEST_METHOD(FreeVirtualMemory);
        TEST_METHOD(ProtectVirtualMemory);
        TEST_METHOD(QueryVirtualMemory);
        TEST_METHOD(ReadVirtualMemory);
        TEST_METHOD(WriteVirtualMemory);
    private:
        DWORD        _currentProcessId;
        HANDLE       _currentProcessHandle;
        std::wstring _currentProcessName;

        MemoryManagerInternal _memoryManagerInternal{};

        std::wstring GetCurrentProcessName();
    };
}
