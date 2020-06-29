#include "MemoryManager.h"
#include "External.h"

namespace MemoryCommando::External {
    MemoryManager::MemoryManager(DWORD processId, DWORD processAccess) {
        _process = External::GetProcess(processId);
        _processHandle = External::GetProcessHandle(processId, processAccess);
    }

    MemoryManager::MemoryManager(const std::wstring& processName, size_t processNumber, DWORD processAccess) {
        _process = External::GetProcess(processName, processNumber);
        _processHandle = External::GetProcessHandle(processName, processNumber, processAccess);
    }

    PROCESSENTRY32W MemoryManager::GetProcess() {
        return _process;
    }

    DWORD MemoryManager::GetProcessId() {
        return _process.th32ProcessID;
    }

    HANDLE MemoryManager::GetProcessHandle() {
        return _processHandle;
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
        return External::AllocateVirtualMemory(_processHandle, baseAddress, allocationSize, allocationType, protectionType);
    }

    void MemoryManager::FreeVirtualMemory(uintptr_t baseAddress, DWORD freeType, size_t size) {
        External::FreeVirtualMemory(_processHandle, baseAddress, freeType, size);
    }

    void MemoryManager::ProtectVirtualMemory(uintptr_t baseAddress, size_t protectionSize, DWORD protectionType) {
        External::ProtectVirtualMemory(_processHandle, baseAddress, protectionSize, protectionType);
    }

    MEMORY_BASIC_INFORMATION MemoryManager::QueryVirtualMemory(uintptr_t baseAddress) {
        return External::QueryVirtualMemory(_processHandle, baseAddress);
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(uintptr_t baseAddress, size_t bytesNumber) {
        return External::ReadVirtualMemory(_processHandle, baseAddress, bytesNumber);
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(std::vector<uintptr_t> pointers, size_t bytesNumber) {
        return External::ReadVirtualMemory(_processHandle, pointers, bytesNumber);
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets, int bytesNumber) {
        return External::ReadVirtualMemory(_processHandle, baseAddress, offsets, bytesNumber);
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(std::wstring moduleName, uintptr_t offset, size_t bytesNumber) {
        return External::ReadVirtualMemory(_processHandle, moduleName, offset, bytesNumber);
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, size_t bytesNumber) {
        return External::ReadVirtualMemory(_processHandle, moduleName, offsets, bytesNumber);
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(uintptr_t baseAddress) {
        return External::ReadVirtualMemory<TStructure>(_processHandle, baseAddress);
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(std::vector<uintptr_t> pointers) {
        return External::ReadVirtualMemory<TStructure>(_processHandle, pointers);
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets) {
        return External::ReadVirtualMemory<TStructure>(_processHandle, baseAddress, offsets);
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(std::wstring moduleName, uintptr_t offset) {
        return External::ReadVirtualMemory<TStructure>(_processHandle, moduleName, offset);
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets) {
        return External::ReadVirtualMemory<TStructure>(_processHandle, moduleName, offsets);
    }

    void MemoryManager::WriteVirtualMemory(uintptr_t baseAddress, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle, baseAddress, byteSequence);
    }

    void MemoryManager::WriteVirtualMemory(std::vector<uintptr_t> pointers, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle, pointers, byteSequence);
    }

    void MemoryManager::WriteVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle, baseAddress, offsets, byteSequence);
    }

    void MemoryManager::WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle, moduleName, offset, byteSequence);
    }

    void MemoryManager::WriteVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle, moduleName, offsets, byteSequence);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(uintptr_t baseAddress, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle, baseAddress, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(std::vector<uintptr_t> pointers, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle, pointers, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> pointers, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle, baseAddress, pointers, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle, moduleName, offset, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle, moduleName, offsets, structure);
    }

    uintptr_t MemoryManager::GetAddress(std::vector<uintptr_t> pointers) {
        return External::GetAddress(_processHandle, pointers);
    }

    uintptr_t MemoryManager::GetAddress(uintptr_t baseAddress, std::vector<uintptr_t> pointers) {
        return External::GetAddress(_processHandle, baseAddress, pointers);
    }

    uintptr_t MemoryManager::GetAddress(std::wstring moduleName, uintptr_t offset) {
        return External::GetAddress(_processHandle, moduleName, offset);
    }

    uintptr_t MemoryManager::GetAddress(std::wstring moduleName, std::vector<uintptr_t> offsets) {
        return External::GetAddress(_processHandle, moduleName, offsets);
    }
}
