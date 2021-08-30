#include "MemoryCommando.h"

#include <stdexcept>

#include "Exceptions/VirtualAllocException.h"
#include "Memory/External/MemoryManagerExternal.h"
#include "Memory/Internal/MemoryManagerInternal.h"
#include "Memory/Scan/MemoryScannerExternal.h"
#include "Memory/Scan/MemoryScannerInternal.h"

namespace MemoryCommando {
    using namespace Internal;
    using namespace External;

    MemoryCommando::MemoryCommando() {
        auto memoryManager = std::make_shared<MemoryManagerInternal>();
        _memoryManager = memoryManager;
        _memoryScannerAbstract = std::make_shared<MemoryScannerInternal>(memoryManager);
    }

    MemoryCommando::MemoryCommando(const DWORD processId) {
        auto memoryManager = std::make_shared<MemoryManagerExternal>(processId);
        _memoryManager = memoryManager;
        _memoryScannerAbstract = std::make_shared<MemoryScannerExternal>(memoryManager);
    }

    MemoryCommando::MemoryCommando(const std::wstring& processName, const size_t processNumber) {
        auto memoryManager = std::make_shared<MemoryManagerExternal>(processName, processNumber);
        _memoryManager = memoryManager;
        _memoryScannerAbstract = std::make_shared<MemoryScannerExternal>(memoryManager);
    }

    PROCESSENTRY32W MemoryCommando::GetProcess() const {
        return _memoryManager->GetProcess();
    }

    std::wstring MemoryCommando::GetProcessName() const {
        return _memoryManager->GetProcessName();
    }

    std::vector<MODULEENTRY32W> MemoryCommando::GetModules() const {
        return _memoryManager->GetModules();
    }

    MODULEENTRY32W MemoryCommando::GetModule(const size_t& moduleIndex) const {
        return _memoryManager->GetModule(moduleIndex);
    }

    MODULEENTRY32W MemoryCommando::GetModule(const std::wstring& moduleName) const {
        return _memoryManager->GetModule(moduleName);
    }

    uintptr_t MemoryCommando::GetModuleBaseAddress(const size_t& moduleIndex) const {
        return _memoryManager->GetModuleBaseAddress(moduleIndex);
    }

    uintptr_t MemoryCommando::GetModuleBaseAddress(const std::wstring& moduleName) const {
        return _memoryManager->GetModuleBaseAddress(moduleName);
    }

    size_t MemoryCommando::GetModuleSize(const size_t& moduleIndex) const {
        return _memoryManager->GetModuleSize(moduleIndex);
    }

    size_t MemoryCommando::GetModuleSize(const std::wstring& moduleName) const {
        return _memoryManager->GetModuleSize(moduleName);
    }

    DWORD MemoryCommando::GetProcessId() const {
        return _memoryManager->GetProcessId();
    }

    HANDLE MemoryCommando::GetProcessHandle() const {
        return _memoryManager->GetProcessHandle();
    }

    uintptr_t MemoryCommando::AllocateVirtualMemory(const uintptr_t baseAddress, const size_t allocationSize, const DWORD protectionType, const DWORD allocationType) const {
        return _memoryManager->AllocateVirtualMemory(baseAddress, allocationSize, protectionType, allocationType);
    }

    uintptr_t MemoryCommando::AllocateVirtualMemoryNear(uintptr_t baseAddress, size_t allocationSize, DWORD allocationType, DWORD protectionType) const {
        uintptr_t allocationAddress = baseAddress;

        while(true) {
            const auto memoryRegion = QueryVirtualMemory(allocationAddress);

            if(!(memoryRegion.State == MEM_FREE)) {
                allocationAddress = uintptr_t(memoryRegion.BaseAddress) + memoryRegion.RegionSize;
                continue;
            }
            try {
                allocationAddress = AllocateVirtualMemory(allocationAddress, allocationSize, allocationType, protectionType);
            }
            catch(Exceptions::VirtualAllocException& exception) {
                if(exception.GetErrorCode() == ERROR_INVALID_ADDRESS) {
                    allocationAddress = uintptr_t(memoryRegion.BaseAddress) + memoryRegion.RegionSize;
                    continue;
                }
                throw;
            }

            break;
        }

        return allocationAddress;
    }

    void MemoryCommando::FreeVirtualMemory(const uintptr_t baseAddress, const DWORD freeType, const size_t size) const {
        return _memoryManager->FreeVirtualMemory(baseAddress, freeType, size);
    }

    void MemoryCommando::ProtectVirtualMemory(const uintptr_t baseAddress, const size_t protectionSize, const DWORD protectionType) const {
        return _memoryManager->ProtectVirtualMemory(baseAddress, protectionSize, protectionType);
    }

    MEMORY_BASIC_INFORMATION MemoryCommando::QueryVirtualMemory(const uintptr_t baseAddress) const {
        return _memoryManager->QueryVirtualMemory(baseAddress);
    }

    std::vector<BYTE> MemoryCommando::ReadVirtualMemory(const uintptr_t baseAddress, const size_t bytesNumber) const {
        return _memoryManager->ReadVirtualMemory(baseAddress, bytesNumber);
    }

    std::vector<BYTE> MemoryCommando::ReadVirtualMemory(const std::vector<uintptr_t>& pointers, const size_t bytesNumber) const {
        return _memoryManager->ReadVirtualMemory(pointers, bytesNumber);
    }

    std::vector<BYTE> MemoryCommando::ReadVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, const int bytesNumber) const {
        return _memoryManager->ReadVirtualMemory(baseAddress, offsets, bytesNumber);
    }

    std::vector<BYTE> MemoryCommando::ReadVirtualMemory(const std::wstring& moduleName, const uintptr_t offset, const size_t bytesNumber) const {
        return _memoryManager->ReadVirtualMemory(moduleName, offset, bytesNumber);
    }

    std::vector<BYTE> MemoryCommando::ReadVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const size_t bytesNumber) const {
        return _memoryManager->ReadVirtualMemory(moduleName, offsets, bytesNumber);
    }

    void MemoryCommando::WriteVirtualMemory(const uintptr_t baseAddress, const std::vector<BYTE>& byteSequence) const {
        _memoryManager->WriteVirtualMemory(baseAddress, byteSequence);
    }

    void MemoryCommando::WriteVirtualMemory(const std::vector<uintptr_t>& pointers, const std::vector<BYTE>& byteSequence) const {
        _memoryManager->WriteVirtualMemory(pointers, byteSequence);
    }

    void MemoryCommando::WriteVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, const std::vector<BYTE>& byteSequence) const {
        _memoryManager->WriteVirtualMemory(baseAddress, offsets, byteSequence);
    }

    void MemoryCommando::WriteVirtualMemory(const std::wstring& moduleName, const uintptr_t offset, const std::vector<BYTE>& byteSequence) const {
        _memoryManager->WriteVirtualMemory(moduleName, offset, byteSequence);
    }

    void MemoryCommando::WriteVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const std::vector<BYTE>& byteSequence) const {
        _memoryManager->WriteVirtualMemory(moduleName, offsets, byteSequence);
    }

    uintptr_t MemoryCommando::GetAddress(const std::vector<uintptr_t>& pointers) const {
        return _memoryManager->GetAddress(pointers);
    }

    uintptr_t MemoryCommando::GetAddress(const uintptr_t baseAddress, const std::vector<uintptr_t>& offsets) const {
        return _memoryManager->GetAddress(baseAddress, offsets);
    }

    uintptr_t MemoryCommando::GetAddress(const std::wstring& moduleName, const uintptr_t offset) const {
        return _memoryManager->GetAddress(moduleName, offset);
    }

    uintptr_t MemoryCommando::GetAddress(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets) const {
        return _memoryManager->GetAddress(moduleName, offsets);
    }

    std::vector<uintptr_t> MemoryCommando::ScanVirtualMemory(const uintptr_t scanStartAddress, const uintptr_t scanEndAddress, const std::string& bytePattern) const {
        return _memoryScannerAbstract->ScanVirtualMemory(scanStartAddress, scanEndAddress, bytePattern);
    }

    std::vector<uintptr_t> MemoryCommando::ScanVirtualMemory(const uintptr_t scanStartAddress, const std::string& bytePattern) const {
        return _memoryScannerAbstract->ScanVirtualMemory(scanStartAddress, bytePattern);
    }

    std::vector<uintptr_t> MemoryCommando::ScanVirtualMemory(const std::string& bytePattern) const {
        return _memoryScannerAbstract->ScanVirtualMemory(bytePattern);
    }

    std::vector<uintptr_t> MemoryCommando::ScanVirtualMemory(const std::wstring& moduleName, const std::string& bytePattern) const {
        return _memoryScannerAbstract->ScanVirtualMemory(moduleName, bytePattern);
    }

    std::vector<uintptr_t> MemoryCommando::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::string& bytePattern) const {
        return _memoryScannerAbstract->ScanVirtualMemory(moduleNames, bytePattern);
    }

    std::vector<uintptr_t> MemoryCommando::ScanVirtualMemory(const uintptr_t scanStartAddress, const uintptr_t scanEndAddress, const std::vector<BYTE>& bytePattern) const {
        return _memoryScannerAbstract->ScanVirtualMemory(scanStartAddress, scanEndAddress, bytePattern);
    }

    std::vector<uintptr_t> MemoryCommando::ScanVirtualMemory(const uintptr_t scanStartAddress, const std::vector<BYTE>& bytePattern) const {
        return _memoryScannerAbstract->ScanVirtualMemory(scanStartAddress, bytePattern);
    }

    std::vector<uintptr_t> MemoryCommando::ScanVirtualMemory(const std::vector<BYTE>& bytePattern) const {
        return _memoryScannerAbstract->ScanVirtualMemory(bytePattern);
    }

    std::vector<uintptr_t> MemoryCommando::ScanVirtualMemory(const std::wstring& moduleName, const std::vector<BYTE>& bytePattern) const {
        return _memoryScannerAbstract->ScanVirtualMemory(moduleName, bytePattern);
    }

    std::vector<uintptr_t> MemoryCommando::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::vector<BYTE>& bytePattern) const {
        return _memoryScannerAbstract->ScanVirtualMemory(moduleNames, bytePattern);
    }
}
