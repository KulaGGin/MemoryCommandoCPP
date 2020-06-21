#include "pch.h"
#include "CppUnitTest.h"
#include "../MemoryCommanderCPP/MemoryManagerExternal.h"
#include <Psapi.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MemoryCommanderCPPTests {
	using namespace MemoryCommanderCpp;
	TEST_CLASS(MemoryManagerExternalTests)	{
	public:
		TEST_METHOD(ConstructorProcessId);
		TEST_METHOD(ConstructorProcessNameWide);
		TEST_METHOD(ConstructorProcessNameNarrow);
	};

    void MemoryManagerExternalTests::ConstructorProcessId() {
        const DWORD currentProcessId = GetCurrentProcessId();

        const MemoryManagerExternal memoryManagerExternal(currentProcessId);
		Assert::IsTrue(memoryManagerExternal.processHandle);
    }

    void MemoryManagerExternalTests::ConstructorProcessNameWide() {
		const DWORD currentProcessId = GetCurrentProcessId();
		HANDLE currentProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, currentProcessId);
		LPSTR currentProcessNameBuffer = new CHAR[MAX_PATH];

		//GetProcessImageFileNameA(currentProcessHandle, currentProcessNameBuffer, MAX_PATH);

		GetModuleBaseNameA(currentProcessHandle, nullptr, currentProcessNameBuffer, MAX_PATH);

		std::string currentProcessName = static_cast<std::string>(currentProcessNameBuffer);

        const std::wstring wCurrentProcessName(currentProcessName.begin(), currentProcessName.end());
        const MemoryManagerExternal memoryManagerExternal(wCurrentProcessName);

		delete[] currentProcessNameBuffer;
		Assert::IsTrue(memoryManagerExternal.processHandle);
    }

	void MemoryManagerExternalTests::ConstructorProcessNameNarrow() {
		const DWORD currentProcessId = GetCurrentProcessId();
		HANDLE currentProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, currentProcessId);
		LPSTR currentProcessNameBuffer = new CHAR[MAX_PATH];

		//GetProcessImageFileNameA(currentProcessHandle, currentProcessNameBuffer, MAX_PATH);

		GetModuleBaseNameA(currentProcessHandle, nullptr, currentProcessNameBuffer, MAX_PATH);

        const std::string currentProcessName = static_cast<std::string>(currentProcessNameBuffer);
		const MemoryManagerExternal memoryManagerExternal(currentProcessName);

		delete[] currentProcessNameBuffer;
		Assert::IsTrue(memoryManagerExternal.processHandle);
	}




}
