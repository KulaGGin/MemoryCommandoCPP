// ReSharper disable CppMemberFunctionMayBeConst
#include "MemoryManagerTests.h"

#include <boost/locale/encoding_utf.hpp>
#include <boost/algorithm/string.hpp>
#include <Psapi.h>

#include "MemoryCommando/Memory/External/MemoryManagerExternal.h"

namespace MemoryCommandoTests {
    using namespace External;
    using namespace Microsoft::VisualStudio::CppUnitTestFramework;

    void MemoryManagerTests::GetModules() {
        const auto modules = _memoryManager->GetModules();

        Assert::IsTrue(!modules.empty());
    }

    MemoryManagerTests::MemoryManagerTests() :
    _currentProcessId(GetCurrentProcessId()),
    _currentProcessHandle(OpenProcess(PROCESS_ALL_ACCESS, 0, _currentProcessId)),
    _currentProcessName(GetCurrentProcessName()),
    _memoryManager{ GetMemoryManager(_currentProcessId) }{
    }

    std::wstring MemoryManagerTests::GetCurrentProcessName() {
        const auto fileNamePointer = std::make_unique<CHAR[]>(MAX_PATH);

        GetModuleBaseNameA(_currentProcessHandle, nullptr, fileNamePointer.get(), MAX_PATH * sizeof(CHAR));
        std::wstring processName = boost::locale::conv::utf_to_utf<WCHAR>(fileNamePointer.get());
        return processName;
    }

    std::shared_ptr<MemoryManager> MemoryManagerTests::GetMemoryManager(const DWORD processId) {
        auto memoryManager = std::make_shared<MemoryManagerExternal>(processId);
        return memoryManager;
    }
}
