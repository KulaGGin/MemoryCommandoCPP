#include <windows.h>

#include "MemoryManagerExternalTests.h"

#include "CppUnitTest.h"

#include "../MemoryCommando/Memory/External/MemoryManagerExternal.h"
#include "../MemoryCommando/Exceptions/Process32Exception.h"



#include <Psapi.h>

#include <boost/locale/encoding_utf.hpp>
#include <boost/algorithm/string.hpp>



using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MemoryCommandoTests {
    using namespace MemoryCommando;
    using namespace Exceptions;
    using namespace Memory::External;

    MemoryManagerExternalTests::MemoryManagerExternalTests() :
    _currentProcessId(GetCurrentProcessId()),
    _currentProcessHandle(OpenProcess(PROCESS_ALL_ACCESS, 0, _currentProcessId)),
    _currentProcessName(GetCurrentProcessName()),
    _memoryManagerExternal(_currentProcessId) {
    }

    void MemoryManagerExternalTests::ConstructorProcessId() {
        const MemoryManagerExternal memoryManagerExternal(_currentProcessId);
		Assert::IsTrue(memoryManagerExternal._processHandle.get());
    }

    void MemoryManagerExternalTests::ConstructorProcessNameWide() {
        const std::wstring wCurrentProcessName(_currentProcessName.begin(), _currentProcessName.end());

        const MemoryManagerExternal memoryManagerExternal(wCurrentProcessName);

		Assert::IsTrue(memoryManagerExternal._processHandle.get());
    }

    void MemoryManagerExternalTests::ConstructorNonExistentName() {
        try {
            const MemoryManagerExternal memoryManagerExternal(L"Non existent process name.");
        }
        catch(const std::runtime_error&) {
            Assert::IsTrue(true);
            return;
        }

        Assert::IsTrue(false);
    }

    void MemoryManagerExternalTests::GetModulesHandles() {
        const MemoryManagerExternal memoryManagerExternal(_currentProcessId);
        const auto modules = memoryManagerExternal.GetModules();

        Assert::IsTrue(!modules.empty());
    }

    std::string MemoryManagerExternalTests::GetCurrentProcessName() {
        std::unique_ptr<CHAR[]> fileNamePointer(new CHAR[MAX_PATH]);
        
        GetModuleBaseNameA(_currentProcessHandle, NULL, fileNamePointer.get(), MAX_PATH * sizeof(CHAR));
        std::string processName = boost::locale::conv::utf_to_utf<CHAR>(fileNamePointer.get());
        return processName;
    }
}
