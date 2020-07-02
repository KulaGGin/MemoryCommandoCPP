#include "MemoryManager.h"
#include "External.h"
#include "Memory.h"

namespace MemoryCommando::External {
    MemoryManager::MemoryManager(DWORD processId, DWORD processAccess) {
        _process = External::GetProcess(processId);
        _processHandle = wil::unique_handle(External::GetProcessHandle(processId, processAccess));
    }

    MemoryManager::MemoryManager(const std::wstring& processName, size_t processNumber, DWORD processAccess) {
        _process = External::GetProcess(processName, processNumber);
        _processHandle = wil::unique_handle(External::GetProcessHandle(processName, processNumber, processAccess));
    }

    PROCESSENTRY32W MemoryManager::GetProcess() {
        return _process;
    }

    DWORD MemoryManager::GetProcessId() {
        return _process.th32ProcessID;
    }

    SYSTEM_INFO MemoryManager::GetSystemInfo() {
        const SYSTEM_INFO systemInfo = Memory::GetSystemInfo();
        return systemInfo;
    }

    HANDLE MemoryManager::GetProcessHandle() {
        return _processHandle.get();
    }

    std::wstring MemoryManager::GetProcessName() {
        return _process.szExeFile;
    }

    std::vector<MODULEENTRY32W> MemoryManager::GetModules() const {
        return External::GetModules(_process.th32ProcessID);
    }

    MODULEENTRY32W MemoryManager::GetModule(const std::wstring& moduleName) {
        return External::GetModule(moduleName, _process.th32ProcessID);
    }

    uintptr_t MemoryManager::GetModuleBaseAddress(const std::wstring& moduleName) {
        return External::GetModuleBaseAddress(moduleName, _process.th32ProcessID);;
    }

    size_t MemoryManager::GetModuleSize(const std::wstring& moduleName) {
        return External::GetModuleSize(_process.th32ProcessID, moduleName);
    }

    uintptr_t MemoryManager::AllocateVirtualMemory(uintptr_t baseAddress, size_t allocationSize, DWORD allocationType, DWORD protectionType) {
        return External::AllocateVirtualMemory(_processHandle.get(), baseAddress, allocationSize, allocationType, protectionType);
    }

    void MemoryManager::FreeVirtualMemory(uintptr_t baseAddress, DWORD freeType, size_t size) {
        External::FreeVirtualMemory(_processHandle.get(), baseAddress, freeType, size);
    }

    void MemoryManager::ProtectVirtualMemory(uintptr_t baseAddress, size_t protectionSize, DWORD protectionType) {
        External::ProtectVirtualMemory(_processHandle.get(), baseAddress, protectionSize, protectionType);
    }

    MEMORY_BASIC_INFORMATION MemoryManager::QueryVirtualMemory(uintptr_t baseAddress) {
        return External::QueryVirtualMemory(_processHandle.get(), baseAddress);
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(uintptr_t baseAddress, size_t bytesNumber) {
        return External::ReadVirtualMemory(_processHandle.get(), baseAddress, bytesNumber);
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(std::vector<uintptr_t> pointers, size_t bytesNumber) {
        return External::ReadVirtualMemory(_processHandle.get(), pointers, bytesNumber);
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets, int bytesNumber) {
        return External::ReadVirtualMemory(_processHandle.get(), baseAddress, offsets, bytesNumber);
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(std::wstring moduleName, uintptr_t offset, size_t bytesNumber) {
        return External::ReadVirtualMemory(_processHandle.get(), moduleName, offset, bytesNumber);
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, size_t bytesNumber) {
        return External::ReadVirtualMemory(_processHandle.get(), moduleName, offsets, bytesNumber);
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(uintptr_t baseAddress) {
        return External::ReadVirtualMemory<TStructure>(_processHandle.get(), baseAddress);
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(std::vector<uintptr_t> pointers) {
        return External::ReadVirtualMemory<TStructure>(_processHandle.get(), pointers);
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets) {
        return External::ReadVirtualMemory<TStructure>(_processHandle.get(), baseAddress, offsets);
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(std::wstring moduleName, uintptr_t offset) {
        return External::ReadVirtualMemory<TStructure>(_processHandle.get(), moduleName, offset);
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets) {
        return External::ReadVirtualMemory<TStructure>(_processHandle.get(), moduleName, offsets);
    }

    void MemoryManager::WriteVirtualMemory(uintptr_t baseAddress, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle.get(), baseAddress, byteSequence);
    }

    void MemoryManager::WriteVirtualMemory(std::vector<uintptr_t> pointers, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle.get(), pointers, byteSequence);
    }

    void MemoryManager::WriteVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle.get(), baseAddress, offsets, byteSequence);
    }

    void MemoryManager::WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle.get(), moduleName, offset, byteSequence);
    }

    void MemoryManager::WriteVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle.get(), moduleName, offsets, byteSequence);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(uintptr_t baseAddress, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle.get(), baseAddress, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(std::vector<uintptr_t> pointers, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle.get(), pointers, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> pointers, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle.get(), baseAddress, pointers, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle.get(), moduleName, offset, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle.get(), moduleName, offsets, structure);
    }

    uintptr_t MemoryManager::GetAddress(std::vector<uintptr_t> pointers) {
        return External::GetAddress(_processHandle.get(), pointers);
    }

    uintptr_t MemoryManager::GetAddress(uintptr_t baseAddress, std::vector<uintptr_t> pointers) {
        return External::GetAddress(_processHandle.get(), baseAddress, pointers);
    }

    uintptr_t MemoryManager::GetAddress(std::wstring moduleName, uintptr_t offset) {
        return External::GetAddress(_processHandle.get(), moduleName, offset);
    }

    uintptr_t MemoryManager::GetAddress(std::wstring moduleName, std::vector<uintptr_t> offsets) {
        return External::GetAddress(_processHandle.get(), moduleName, offsets);
    }
}
