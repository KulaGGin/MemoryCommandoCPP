// ReSharper disable CppMemberFunctionMayBeConst
#include "MemoryManagerInternalTests.h"


#include <boost/locale/encoding_utf.hpp>
#include <Psapi.h>

#include "Exceptions/WinAPIException.h"
#include "HelperMethods.h"

namespace MemoryCommandoTests {
    using namespace Exceptions;

    MemoryManagerInternalTests::MemoryManagerInternalTests():
        _currentProcessId(GetCurrentProcessId()),
        _currentProcessHandle(OpenProcess(PROCESS_ALL_ACCESS, 0, _currentProcessId)),
        _currentProcessName(GetCurrentProcessName()) {
    }

    void MemoryManagerInternalTests::Constructor() {
        const MemoryManagerInternal memoryManagerInternal{};

        if(!memoryManagerInternal._processHandle.get())
            Assert::IsTrue(false);
        if(memoryManagerInternal._processId != _currentProcessId)
            Assert::IsTrue(false);
        if(memoryManagerInternal._process.szExeFile != _currentProcessName)
            Assert::IsTrue(false);
    }

    void MemoryManagerInternalTests::GetProcessId() {
        const DWORD currentProcessId = _memoryManagerInternal.GetProcessId();

        if(currentProcessId != _currentProcessId)
            Assert::IsTrue(false);
    }

    void MemoryManagerInternalTests::GetProcessHandle() {
        const auto processHandle = _memoryManagerInternal.GetProcessHandle();
        Assert::IsTrue(processHandle);
    }

    void MemoryManagerInternalTests::AllocateVirtualMemory() {
        const auto allocatedAddress = _memoryManagerInternal.AllocateVirtualMemory(0, 0x1000);
        Assert::IsTrue(allocatedAddress);
    }

    void MemoryManagerInternalTests::FreeVirtualMemory() {
        try {
            const auto allocatedAddress = _memoryManagerInternal.AllocateVirtualMemory(0, 0x1000);
            _memoryManagerInternal.FreeVirtualMemory(allocatedAddress);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void MemoryManagerInternalTests::ProtectVirtualMemory() {
        try {
            const auto allocatedAddress = _memoryManagerInternal.AllocateVirtualMemory(0, 0x1000);
            _memoryManagerInternal.ProtectVirtualMemory(allocatedAddress, 0x1000);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void MemoryManagerInternalTests::QueryVirtualMemory() {
        try {
            const auto allocatedAddress = _memoryManagerInternal.AllocateVirtualMemory(0, 0x1000);
            auto memoryInformation = _memoryManagerInternal.QueryVirtualMemory(allocatedAddress);
            if(uintptr_t(memoryInformation.AllocationBase) != allocatedAddress)
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void MemoryManagerInternalTests::ReadVirtualMemory() {
        try {
            unsigned long long testInteger = 0x01234ABCDEF;
            unsigned long long readInteger;
            auto readBytes = _memoryManagerInternal.ReadVirtualMemory(uintptr_t(&testInteger), sizeof testInteger);
            std::memcpy(&readInteger, &readBytes[0], sizeof readInteger);
            if(readInteger != testInteger)
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    void MemoryManagerInternalTests::WriteVirtualMemory() {
        try {
            const int  testInteger = 0x1234;
            const auto integerBytes = HelperMethods::ConvertObjectToBytes(testInteger);
            const auto integerPointer = std::make_unique<int>();
            _memoryManagerInternal.WriteVirtualMemory(uintptr_t(integerPointer.get()), integerBytes);
            if(testInteger != *integerPointer)
                Assert::IsTrue(false);
        }
        catch(WinAPIException&) {
            Assert::IsTrue(false);
        }
    }

    std::wstring MemoryManagerInternalTests::GetCurrentProcessName() {
        const std::unique_ptr<CHAR[]> fileNamePointer(new CHAR[MAX_PATH]);

        GetModuleBaseNameA(_currentProcessHandle, nullptr, fileNamePointer.get(), MAX_PATH * sizeof(CHAR));
        std::wstring processName = boost::locale::conv::utf_to_utf<WCHAR>(fileNamePointer.get());

        return processName;
    }
}
