#include "CppUnitTest.h"
#include "../MemoryCommando/MemoryManager.h"
#include <Psapi.h>
#include "MemoryManagerExternalTests.h"

#include "../MemoryCommando/Process32Exception.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MemoryCommandoTests {
    using namespace Exceptions;
    using namespace External;

    MemoryManagerExternalTests::MemoryManagerExternalTests() {
        _currentProcessId = GetCurrentProcessId();
        _currentProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, _currentProcessId);
        _currentProcessName = GetCurrentProcessName();
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

	void MemoryManagerExternalTests::ConstructorProcessNameNarrow() {
		const MemoryManager memoryManagerExternal(_currentProcessName);

		Assert::IsTrue(memoryManagerExternal._processHandle);
	}

    void MemoryManagerExternalTests::ConstructorNonExistentName() {
        try {
            const MemoryManager memoryManagerExternal("Non existent process name.");
        }
        catch(const Process32Exception&) {
            Assert::IsTrue(true);
            return;
        }

        Assert::IsTrue(false);
    }

    void MemoryManagerExternalTests::GetModulesHandles() {
        MemoryManager memoryManagerExternal(_currentProcessId);
        std::vector<HMODULE> modules = memoryManagerExternal.GetModulesHandles();

        Assert::IsTrue(modules.size());
    }

    std::string MemoryManagerExternalTests::GetCurrentProcessName() {
        const auto currentProcessNameSmartBuffer = std::shared_ptr<char[]>(new CHAR[MAX_PATH]);

        GetModuleBaseNameA(_currentProcessHandle, nullptr, currentProcessNameSmartBuffer.get(), MAX_PATH);

        const std::string currentProcessName = static_cast<std::string>(currentProcessNameSmartBuffer.get());

        return currentProcessName;
    }
}
