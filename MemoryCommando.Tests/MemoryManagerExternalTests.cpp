#pragma once
#include <windows.h>

#include "CppUnitTest.h"

#include "../MemoryCommando/External.h"

#include "../MemoryCommando/MemoryManager.h"
#include "../MemoryCommando/Process32Exception.h"

#include "MemoryManagerExternalTests.h"

#include <Psapi.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MemoryCommandoTests {
    using namespace MemoryCommando;
    using namespace Exceptions;
    using namespace External;

    MemoryManagerExternalTests::MemoryManagerExternalTests() {
        _currentProcessId = GetCurrentProcessId();
        _currentProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, _currentProcessId);
        _currentProcessName = GetProcessName(_currentProcessId);
    }

    void MemoryManagerExternalTests::ConstructorProcessId() {
        const MemoryManager memoryManagerExternal(_currentProcessId);
		Assert::IsTrue(memoryManagerExternal._processHandle);
    }

    void MemoryManagerExternalTests::ConstructorProcessNameWide() {
        const std::wstring wCurrentProcessName(_currentProcessName.begin(), _currentProcessName.end());

        const MemoryManager memoryManagerExternal(wCurrentProcessName);

		Assert::IsTrue(memoryManagerExternal._processHandle);
    }

    void MemoryManagerExternalTests::ConstructorNonExistentName() {
        try {
            const MemoryManager memoryManagerExternal(L"Non existent process name.");
        }
        catch(const std::runtime_error&) {
            Assert::IsTrue(true);
            return;
        }

        Assert::IsTrue(false);
    }

    void MemoryManagerExternalTests::GetModulesHandles() {
        const MemoryManager memoryManagerExternal(_currentProcessId);
        const auto modules = memoryManagerExternal.GetModules();

        Assert::IsTrue(!modules.empty());
    }
}
