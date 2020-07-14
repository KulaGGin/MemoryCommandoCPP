#pragma once
#include <CppUnitTest.h>
#include <minwindef.h>
#include <strsafe.h>

#include "MemoryCommando/Memory/External/MemoryManagerExternal.h"

namespace MemoryCommandoTests {
    using namespace MemoryCommando::Memory::External;
    TEST_CLASS(MemoryManagerExternalTests) {
    public:
        MemoryManagerExternalTests();

        TEST_METHOD(ConstructorProcessId);
        TEST_METHOD(ConstructorProcessNameWide);
        TEST_METHOD(ConstructorNonExistentName);
        TEST_METHOD(GetProcessId);
        TEST_METHOD(GetProcessHandle);
        TEST_METHOD(GetNewProcessHandle);
        TEST_METHOD(AllocateVirtualMemory);
        TEST_METHOD(FreeVirtualMemory);
        TEST_METHOD(ProtectVirtualMemory);
        TEST_METHOD(QueryVirtualMemory);
        TEST_METHOD(ReadVirtualMemory);
        TEST_METHOD(WriteVirtualMemory);
    private:
        DWORD _currentProcessId;
        HANDLE _currentProcessHandle;
        std::wstring _currentProcessName;

        MemoryManagerExternal _memoryManagerExternal;

        std::wstring GetCurrentProcessName();
    };
}
