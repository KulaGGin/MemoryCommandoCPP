#pragma once

#include <memory>
#include <windows.h>
#include <processthreadsapi.h>
#include <string>

#include "Memory/External/MemoryManagerExternal.h"
#include "Memory/Internal/MemoryManagerInternal.h"

namespace MemoryCommandoTests {
    using namespace MemoryCommando;
    using namespace Memory;
    using namespace External;
    using namespace Internal;
    class MemoryCommandoTestsHelper {
    public:
        MemoryCommandoTestsHelper();
        ~MemoryCommandoTestsHelper();

        DWORD GetCurrentProcessId() const;
        wil::unique_handle& GetsCurrentProcessHandle();
        std::wstring GetCurrentProcessName() const;
        PROCESS_INFORMATION GetTestedProcess();
        std::wstring GetSystemDirectoryPath() const;
        std::shared_ptr<const MemoryManagerExternal> GetMemoryManagerExternal() const;
        MemoryManagerInternal& GetMemoryManagerInternal();
    private:
        DWORD _currentProcessId;
        wil::unique_handle _currentProcessHandle;
        std::wstring _currentProcessName;

        PROCESS_INFORMATION _testedProcessInformation;

        std::shared_ptr<const MemoryManagerExternal> _memoryManagerExternal;
        MemoryManagerInternal _memoryManagerInternal{};

        DWORD InitializeCurrentProcessId() const;
        wil::unique_handle InitializeCurrentProcessHandle() const;
        std::wstring InitializeCurrentProcessName() const;
        PROCESS_INFORMATION InitializeTestedProcess() const;
    };
}

