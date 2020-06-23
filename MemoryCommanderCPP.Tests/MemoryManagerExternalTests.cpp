#include "pch.h"
#include "CppUnitTest.h"
#include "../MemoryCommanderCPP/MemoryManagerExternal.h"
#include <Psapi.h>
#include "MemoryManagerExternalTests.h"

#include "../MemoryCommanderCPP/Process32Exception.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MemoryCommanderCPPTests {
    using namespace MemoryCommanderCpp::Exceptions;

    MemoryManagerExternalTests::MemoryManagerExternalTests() {
        _currentProcessId = GetCurrentProcessId();
        _currentProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, _currentProcessId);
        _currentProcessName = GetCurrentProcessName();
    }

    void MemoryManagerExternalTests::ConstructorProcessId() {
        const MemoryManagerExternal memoryManagerExternal(_currentProcessId);
		Assert::IsTrue(memoryManagerExternal._processHandle);
    }

    void MemoryManagerExternalTests::ConstructorProcessNameWide() {
        const std::wstring wCurrentProcessName(_currentProcessName.begin(), _currentProcessName.end());

        const MemoryManagerExternal memoryManagerExternal(wCurrentProcessName);

		Assert::IsTrue(memoryManagerExternal._processHandle);
    }

	void MemoryManagerExternalTests::ConstructorProcessNameNarrow() {
		const MemoryManagerExternal memoryManagerExternal(_currentProcessName);

		Assert::IsTrue(memoryManagerExternal._processHandle);
	}

    void MemoryManagerExternalTests::ConstructorNonExistentName() {
        try {
            const MemoryManagerExternal memoryManagerExternal("Non existent process name.");
        }
        catch(const Process32Exception&) {
            Assert::IsTrue(true);
            return;
        }

        Assert::IsTrue(false);
    }

    void MemoryManagerExternalTests::GetModulesHandles() {
        MemoryManagerExternal memoryManagerExternal(_currentProcessId);
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