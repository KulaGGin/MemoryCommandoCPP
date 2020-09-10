// ReSharper disable CppMemberFunctionMayBeConst
// ReSharper disable CppMemberFunctionMayBeStatic
#include <windows.h>

#include "MemoryManagerExternalTests.h"

#include "CppUnitTest.h"

#include "MemoryCommando/Memory/External/MemoryManagerExternal.h"

#include <Psapi.h>

#include <boost/locale/encoding_utf.hpp>
#include <boost/algorithm/string.hpp>

#include "MemoryCommando/HelperMethods.h"

#include "MemoryCommando/Exceptions/ReadProcessMemoryException.h"
#include "MemoryCommando/Exceptions/VirtualFreeExException.h"
#include "MemoryCommando/Exceptions/VirtualProtectExException.h"
#include "MemoryCommando/Exceptions/VirtualQueryExException.h"
#include "MemoryCommando/Exceptions/WriteProcessMemoryException.h"


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

        if(!memoryManagerExternal._processHandle.get())
            Assert::IsTrue(false);
        if(memoryManagerExternal._processId != _currentProcessId)
            Assert::IsTrue(false);
        if(memoryManagerExternal._process.szExeFile != _currentProcessName)
            Assert::IsTrue(false);
    }

    void MemoryManagerExternalTests::ConstructorProcessNameWide() {
        const MemoryManagerExternal memoryManagerExternal(_currentProcessName);

        if(!memoryManagerExternal._processHandle.get())
            Assert::IsTrue(false);
        if(memoryManagerExternal._processId != _currentProcessId)
            Assert::IsTrue(false);
        if(memoryManagerExternal._process.szExeFile != _currentProcessName)
            Assert::IsTrue(false);
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

    void MemoryManagerExternalTests::GetProcessId() {
        const DWORD currentProcessId = _memoryManagerExternal.GetProcessId();

        if(currentProcessId != _currentProcessId)
            Assert::IsTrue(false);
    }

    void MemoryManagerExternalTests::GetProcessHandle() {
        const auto processHandle = _memoryManagerExternal.GetProcessHandle();
        Assert::IsTrue(processHandle);
    }

    void MemoryManagerExternalTests::GetNewProcessHandle() {
        const auto processHandle = _memoryManagerExternal.GetNewProcessHandle();
        Assert::IsTrue(processHandle);
    }

    void MemoryManagerExternalTests::AllocateVirtualMemory() {
        const auto allocatedAddress = _memoryManagerExternal.AllocateVirtualMemory(0, 0x1000);
        Assert::IsTrue(allocatedAddress);
    }

    void MemoryManagerExternalTests::FreeVirtualMemory() {
        try {
            const auto allocatedAddress = _memoryManagerExternal.AllocateVirtualMemory(0, 0x1000);
            _memoryManagerExternal.FreeVirtualMemory(allocatedAddress);
        }
        catch(VirtualFreeExException&) {
            Assert::IsTrue(false);
        }
    }

    void MemoryManagerExternalTests::ProtectVirtualMemory() {
        try {
            const auto allocatedAddress = _memoryManagerExternal.AllocateVirtualMemory(0, 0x1000);
            _memoryManagerExternal.ProtectVirtualMemory(allocatedAddress, 0x1000);
        }
        catch(VirtualProtectExException&) {
            Assert::IsTrue(false);
        }
    }

    void MemoryManagerExternalTests::QueryVirtualMemory() {
        try {
            const auto allocatedAddress = _memoryManagerExternal.AllocateVirtualMemory(0, 0x1000);
            auto memoryInformation = _memoryManagerExternal.QueryVirtualMemory(allocatedAddress);
            if(uintptr_t(memoryInformation.AllocationBase) != allocatedAddress)
                Assert::IsTrue(false);
        }
        catch(VirtualQueryExException&) {
            Assert::IsTrue(false);
        }
    }

    void MemoryManagerExternalTests::ReadVirtualMemory() {
        try {
            unsigned long long testInteger = 0x01234ABCDEF;
            unsigned long long readInteger;
            auto readBytes = _memoryManagerExternal.ReadVirtualMemory(uintptr_t(&testInteger), sizeof testInteger);
            std::memcpy(&readInteger, &readBytes[0], sizeof readInteger);
            if(readInteger != testInteger)
                Assert::IsTrue(false);
        }
        catch(ReadProcessMemoryException&) {
            Assert::IsTrue(false);
        }
    }

    void MemoryManagerExternalTests::WriteVirtualMemory() {
        try {
            const int  testInteger = 0x1234;
            const auto integerBytes = HelperMethods::ConvertObjectToBytes(testInteger);
            const auto integerPointer = std::make_unique<int>();
            _memoryManagerExternal.WriteVirtualMemory(uintptr_t(integerPointer.get()), integerBytes);
            if(testInteger != *integerPointer)
                Assert::IsTrue(false);
        }
        catch(WriteProcessMemoryException&) {
            Assert::IsTrue(false);
        }
    }

    std::wstring MemoryManagerExternalTests::GetCurrentProcessName() {
        const auto fileNamePointer = std::make_unique<CHAR[]>(MAX_PATH);
        
        GetModuleBaseNameA(_currentProcessHandle, nullptr, fileNamePointer.get(), MAX_PATH * sizeof(CHAR));
        std::wstring processName = boost::locale::conv::utf_to_utf<WCHAR>(fileNamePointer.get());

        return processName;
    }
}
