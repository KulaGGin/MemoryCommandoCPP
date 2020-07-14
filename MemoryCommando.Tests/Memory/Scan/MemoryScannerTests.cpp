// ReSharper disable CppMemberFunctionMayBeConst
#include "MemoryScannerTests.h"

#include <boost/locale/encoding_utf.hpp>


#include "Exceptions/VirtualQueryExException.h"
#include "Exceptions/WinAPIException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <windows.h>
#include <processthreadsapi.h>
#include <sysinfoapi.h>

namespace MemoryCommandoTests {
    using namespace Memory;

    MemoryScannerTests::MemoryScannerTests():
        _memoryManager(_memoryCommandoTestsHelper.GetMemoryManagerExternal()),
        _memoryScanner(_memoryManager) {
    }

    void MemoryScannerTests::Constructor() {
        MemoryScanner memoryScanner{_memoryManager};
    }

    void MemoryScannerTests::GetAccessibleMemoryRegions() {
        const auto address = _memoryManager.AllocateVirtualMemory(0, 0x5000);
        auto memoryRegions = _memoryScanner.GetAccessibleUsedMemoryRegions(address, address + 0x5000);

        if(memoryRegions.size() != 1)
            Assert::IsTrue(false);
        if(uintptr_t(memoryRegions[0].BaseAddress) != address)
            Assert::IsTrue(false);
        if(memoryRegions[0].RegionSize != 0x5000)
            Assert::IsTrue(false);

        _memoryManager.ProtectVirtualMemory(address + 0x0000, 0x1000, PAGE_READONLY);
        _memoryManager.ProtectVirtualMemory(address + 0x1000, 0x1000, PAGE_READWRITE);
        _memoryManager.ProtectVirtualMemory(address + 0x2000, 0x1000, PAGE_EXECUTE);

        memoryRegions = _memoryScanner.GetAccessibleUsedMemoryRegions(address, address + 0x5000);

        if(memoryRegions.size() != 4)
            Assert::IsTrue(false);
        if(uintptr_t(memoryRegions[0].BaseAddress) != address || memoryRegions[0].RegionSize != 0x1000)
            Assert::IsTrue(false);
        if(uintptr_t(memoryRegions[1].BaseAddress) != address + 0x1000 || memoryRegions[1].RegionSize != 0x1000)
            Assert::IsTrue(false);
        if(uintptr_t(memoryRegions[2].BaseAddress) != address + 0x2000 || memoryRegions[2].RegionSize != 0x1000)
            Assert::IsTrue(false);
        if(uintptr_t(memoryRegions[3].BaseAddress) != address + 0x3000 || memoryRegions[3].RegionSize != 0x2000)
            Assert::IsTrue(false);

        Assert::IsTrue(true);
    }

    void MemoryScannerTests::ScanStringStartEnd() {
        const std::vector<BYTE> byteArray{ 0x43, 0x5C, 0xCA, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC, 0x62, 0xA0, 0x35, 0x34, 0x81 };
        const auto allocatedAddress = _memoryManager.AllocateVirtualMemory(0, 0x1000);
        _memoryManager.WriteVirtualMemory(allocatedAddress, byteArray);
        
        const uintptr_t addressPastTheEnd = allocatedAddress + byteArray.size();
        const std::string pattern = "0x18 0x33 ?? 0xF0 0x18 0x33";

        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(allocatedAddress - 4, addressPastTheEnd + 4, pattern);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != allocatedAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != allocatedAddress + 7)
            Assert::IsTrue(false);
    }

    void MemoryScannerTests::ScanStringStart() {
        const std::vector<BYTE> byteArray{ 0x43, 0x5C, 0xCA, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC, 0x62, 0xA0, 0x35, 0x34, 0x81 };
        const auto allocatedAddress = _memoryManager.AllocateVirtualMemory(0, 0x1000);
        _memoryManager.WriteVirtualMemory(allocatedAddress, byteArray);
        const std::string pattern = "0x18 0x33 ?? 0xF0 0x18 0x33";

        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(allocatedAddress - 4, pattern);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != allocatedAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != allocatedAddress + 7)
            Assert::IsTrue(false);
    }

    void MemoryScannerTests::ScanString() {
        const auto allocatedAddress = _memoryManager.AllocateVirtualMemory(_memoryScanner._minimumApplicationAddress, 0x1000);
        _memoryManager.WriteVirtualMemory(allocatedAddress, _testByteArray);
        const std::string pattern = "0x18 0x33 ?? 0xF0 0x18 0x33";

        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(pattern);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != allocatedAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != allocatedAddress + 7)
            Assert::IsTrue(false);
    }

    void MemoryScannerTests::ScanStringModuleName() {
        auto modules = HelperMethods::GetModules(_memoryCommandoTestsHelper.GetTestedProcess().dwProcessId);
        auto module = modules[0];
        const uintptr_t byteArrayAddress = uintptr_t(module.modBaseAddr + module.modBaseSize) - _testByteArray.size();
        _memoryManager.ProtectVirtualMemory(uintptr_t(module.modBaseAddr), uintptr_t(module.modBaseSize), PAGE_EXECUTE_READWRITE);
        _memoryManager.WriteVirtualMemory(byteArrayAddress, _testByteArray);

        const std::string pattern = "0x18 0x33 ?? 0xF0 0x18 0x33";
        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(module.szModule, pattern);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != byteArrayAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != byteArrayAddress + 7)
            Assert::IsTrue(false);
    }

    void MemoryScannerTests::ScanStringModuleNames() {
        auto modules = HelperMethods::GetModules(_memoryCommandoTestsHelper.GetTestedProcess().dwProcessId);
        auto module = modules[0];
        const uintptr_t byteArrayAddress = uintptr_t(module.modBaseAddr + module.modBaseSize) - _testByteArray.size();
        _memoryManager.ProtectVirtualMemory(uintptr_t(module.modBaseAddr), uintptr_t(module.modBaseSize), PAGE_EXECUTE_READWRITE);
        _memoryManager.WriteVirtualMemory(byteArrayAddress, _testByteArray);

        std::vector<std::wstring> moduleNames;
        moduleNames.reserve(modules.size());
        for(auto currentModule : modules) {
            moduleNames.emplace_back(currentModule.szModule);
        }

        const std::string pattern = "0x18 0x33 ?? 0xF0 0x18 0x33";
        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(moduleNames, pattern);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != byteArrayAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != byteArrayAddress + 7)
            Assert::IsTrue(false);
    }

    void MemoryScannerTests::ScanBytePatternStartEnd() {
        const std::vector<BYTE> byteArray{ 0x43, 0x5C, 0xCA, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC, 0x62, 0xA0, 0x35, 0x34, 0x81 };
        const auto allocatedAddress = _memoryManager.AllocateVirtualMemory(0, 0x1000);
        _memoryManager.WriteVirtualMemory(allocatedAddress, byteArray);

        const uintptr_t addressPastTheEnd = allocatedAddress + byteArray.size();

        const auto pattern = std::vector<BYTE>{ 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33 };

        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(allocatedAddress - 4, addressPastTheEnd + 4, pattern);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != allocatedAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != allocatedAddress + 7)
            Assert::IsTrue(false);
    }

    void MemoryScannerTests::ScanBytePatternStart() {
        const std::vector<BYTE> byteArray{ 0x43, 0x5C, 0xCA, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC, 0x62, 0xA0, 0x35, 0x34, 0x81 };
        const auto allocatedAddress = _memoryManager.AllocateVirtualMemory(0, 0x1000);
        _memoryManager.WriteVirtualMemory(allocatedAddress, byteArray);

        const auto pattern = std::vector<BYTE>{ 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33 };

        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(allocatedAddress - 4, pattern);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != allocatedAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != allocatedAddress + 7)
            Assert::IsTrue(false);
    }

    void MemoryScannerTests::ScanBytePattern() {
        const auto allocatedAddress = _memoryManager.AllocateVirtualMemory(_memoryScanner._minimumApplicationAddress, 0x1000);
        _memoryManager.WriteVirtualMemory(allocatedAddress, _testByteArray);

        const auto pattern = std::vector<BYTE>{ 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33 };

        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(pattern);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != allocatedAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != allocatedAddress + 7)
            Assert::IsTrue(false);
    }

    void MemoryScannerTests::ScanBytePatternModuleName() {
        auto modules = HelperMethods::GetModules(_memoryCommandoTestsHelper.GetTestedProcess().dwProcessId);
        auto module = modules[0];
        const uintptr_t byteArrayAddress = uintptr_t(module.modBaseAddr + module.modBaseSize) - _testByteArray.size();
        _memoryManager.ProtectVirtualMemory(uintptr_t(module.modBaseAddr), uintptr_t(module.modBaseSize), PAGE_EXECUTE_READWRITE);
        _memoryManager.WriteVirtualMemory(byteArrayAddress, _testByteArray);

        const auto pattern = std::vector<BYTE>{ 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33 };
        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(module.szModule, pattern);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != byteArrayAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != byteArrayAddress + 7)
            Assert::IsTrue(false);
    }

    void MemoryScannerTests::ScanBytePatternModuleNames() {
        auto modules = HelperMethods::GetModules(_memoryCommandoTestsHelper.GetTestedProcess().dwProcessId);
        auto module = modules[0];
        const uintptr_t byteArrayAddress = uintptr_t(module.modBaseAddr + module.modBaseSize) - _testByteArray.size();
        _memoryManager.ProtectVirtualMemory(uintptr_t(module.modBaseAddr), uintptr_t(module.modBaseSize), PAGE_EXECUTE_READWRITE);
        _memoryManager.WriteVirtualMemory(byteArrayAddress, _testByteArray);

        std::vector<std::wstring> moduleNames;
        moduleNames.reserve(modules.size());
        for(auto currentModule : modules) {
            moduleNames.emplace_back(currentModule.szModule);
        }

        const auto pattern = std::vector<BYTE>{ 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33 };
        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(moduleNames, pattern);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != byteArrayAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != byteArrayAddress + 7)
            Assert::IsTrue(false);
    }

    void MemoryScannerTests::ScanObjectStartEnd() {
        const std::vector<BYTE> byteArray{ 0x43, 0x5C, 0xCA, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC, 0x62, 0xA0, 0x35, 0x34, 0x81 };
        const auto allocatedAddress = _memoryManager.AllocateVirtualMemory(0, 0x1000);
        _memoryManager.WriteVirtualMemory(allocatedAddress, byteArray);

        const uintptr_t addressPastTheEnd = allocatedAddress + byteArray.size();

        UINT64 object{};
        const auto bytePattern = std::vector<BYTE>{ 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0 };
        std::memcpy(&object, &bytePattern[0], bytePattern.size());

        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(allocatedAddress - 4, addressPastTheEnd + 4, object);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != allocatedAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != allocatedAddress + 7)
            Assert::IsTrue(false);
    }

    void MemoryScannerTests::ScanObjectStart() {
        const std::vector<BYTE> byteArray{ 0x43, 0x5C, 0xCA, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0, 0x31, 0xBC, 0xC4, 0xFC, 0x62, 0xA0, 0x35, 0x34, 0x81 };
        const auto allocatedAddress = _memoryManager.AllocateVirtualMemory(0, 0x1000);
        _memoryManager.WriteVirtualMemory(allocatedAddress, byteArray);

        UINT64 object{};
        const auto bytePattern = std::vector<BYTE>{ 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0 };
        std::memcpy(&object, &bytePattern[0], bytePattern.size());

        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(allocatedAddress - 4, object);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != allocatedAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != allocatedAddress + 7)
            Assert::IsTrue(false);
    }

    void MemoryScannerTests::ScanObject() {
        const auto allocatedAddress = _memoryManager.AllocateVirtualMemory(_memoryScanner._minimumApplicationAddress, 0x1000);
        _memoryManager.WriteVirtualMemory(allocatedAddress, _testByteArray);

        UINT64 object{};
        const auto bytePattern = std::vector<BYTE>{ 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0 };
        std::memcpy(&object, &bytePattern[0], bytePattern.size());

        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(object);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != allocatedAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != allocatedAddress + 7)
            Assert::IsTrue(false);
    }

    void MemoryScannerTests::ScanObjectModuleName() {
        auto modules = HelperMethods::GetModules(_memoryCommandoTestsHelper.GetTestedProcess().dwProcessId);
        auto module = modules[0];
        const uintptr_t byteArrayAddress = uintptr_t(module.modBaseAddr + module.modBaseSize) - _testByteArray.size();
        _memoryManager.ProtectVirtualMemory(uintptr_t(module.modBaseAddr), uintptr_t(module.modBaseSize), PAGE_EXECUTE_READWRITE);
        _memoryManager.WriteVirtualMemory(byteArrayAddress, _testByteArray);

        UINT64 object{};
        const auto bytePattern = std::vector<BYTE>{ 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0 };
        std::memcpy(&object, &bytePattern[0], bytePattern.size());

        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(module.szModule, object);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != byteArrayAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != byteArrayAddress + 7)
            Assert::IsTrue(false);
    }

    void MemoryScannerTests::ScanObjectModuleNames() {
        auto modules = HelperMethods::GetModules(_memoryCommandoTestsHelper.GetTestedProcess().dwProcessId);
        auto module = modules[0];
        const uintptr_t byteArrayAddress = uintptr_t(module.modBaseAddr + module.modBaseSize) - _testByteArray.size();
        _memoryManager.ProtectVirtualMemory(uintptr_t(module.modBaseAddr), uintptr_t(module.modBaseSize), PAGE_EXECUTE_READWRITE);
        _memoryManager.WriteVirtualMemory(byteArrayAddress, _testByteArray);

        std::vector<std::wstring> moduleNames;
        moduleNames.reserve(modules.size());
        for(auto currentModule : modules) {
            moduleNames.emplace_back(currentModule.szModule);
        }

        UINT64 object{};
        const auto bytePattern = std::vector<BYTE>{ 0x18, 0x33, 0x17, 0xF0, 0x18, 0x33, 0x17, 0xF0 };
        std::memcpy(&object, &bytePattern[0], bytePattern.size());

        std::vector<uintptr_t> patternAddresses = _memoryScanner.ScanVirtualMemory(moduleNames, object);

        if(patternAddresses.size() != 2)
            Assert::IsTrue(false);
        if(patternAddresses[0] != byteArrayAddress + 3)
            Assert::IsTrue(false);
        if(patternAddresses[1] != byteArrayAddress + 7)
            Assert::IsTrue(false);
    }
}
