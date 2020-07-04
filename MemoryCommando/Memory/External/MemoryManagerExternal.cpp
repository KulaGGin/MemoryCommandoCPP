#include "MemoryManagerExternal.h"
#include "External.h"
#include "../Memory.h"

namespace MemoryCommando::External {
    MemoryManagerExternal::MemoryManagerExternal(DWORD processId, DWORD processAccess) {
        _process = External::GetProcess(processId);
        _processHandle = wil::unique_handle(External::GetProcessHandle(processId, processAccess));
    }

    MemoryManagerExternal::MemoryManagerExternal(const std::wstring& processName, size_t processNumber, DWORD processAccess) {
        _process = External::GetProcess(processName, processNumber);
        _processHandle = wil::unique_handle(External::GetProcessHandle(processName, processNumber, processAccess));
    }

    PROCESSENTRY32W MemoryManagerExternal::GetProcess() {
        return _process;
    }

    DWORD MemoryManagerExternal::GetProcessId() {
        return _process.th32ProcessID;
    }

    SYSTEM_INFO MemoryManagerExternal::GetSystemInfo() {
        const SYSTEM_INFO systemInfo = Memory::GetSystemInfo();
        return systemInfo;
    }

    HANDLE MemoryManagerExternal::GetProcessHandle() {
        return _processHandle.get();
    }

    std::wstring MemoryManagerExternal::GetProcessName() {
        return _process.szExeFile;
    }

    std::vector<MODULEENTRY32W> MemoryManagerExternal::GetModules() const {
        return External::GetModules(_process.th32ProcessID);
    }

    MODULEENTRY32W MemoryManagerExternal::GetModule(const std::wstring& moduleName) {
        return External::GetModule(moduleName, _process.th32ProcessID);
    }

    uintptr_t MemoryManagerExternal::GetModuleBaseAddress(const std::wstring& moduleName) {
        return External::GetModuleBaseAddress(moduleName, _process.th32ProcessID);;
    }

    size_t MemoryManagerExternal::GetModuleSize(const std::wstring& moduleName) {
        return External::GetModuleSize(_process.th32ProcessID, moduleName);
    }

    uintptr_t MemoryManagerExternal::AllocateVirtualMemory(uintptr_t baseAddress, size_t allocationSize, DWORD allocationType, DWORD protectionType) {
        return External::AllocateVirtualMemory(_processHandle.get(), baseAddress, allocationSize, allocationType, protectionType);
    }

    void MemoryManagerExternal::FreeVirtualMemory(uintptr_t baseAddress, DWORD freeType, size_t size) {
        External::FreeVirtualMemory(_processHandle.get(), baseAddress, freeType, size);
    }

    void MemoryManagerExternal::ProtectVirtualMemory(uintptr_t baseAddress, size_t protectionSize, DWORD protectionType) {
        External::ProtectVirtualMemory(_processHandle.get(), baseAddress, protectionSize, protectionType);
    }

    MEMORY_BASIC_INFORMATION MemoryManagerExternal::QueryVirtualMemory(uintptr_t baseAddress) {
        return External::QueryVirtualMemory(_processHandle.get(), baseAddress);
    }

    std::vector<BYTE> MemoryManagerExternal::ReadVirtualMemory(uintptr_t baseAddress, size_t bytesNumber) {
        return External::ReadVirtualMemory(_processHandle.get(), baseAddress, bytesNumber);
    }

    std::vector<BYTE> MemoryManagerExternal::ReadVirtualMemory(std::vector<uintptr_t> pointers, size_t bytesNumber) {
        return External::ReadVirtualMemory(_processHandle.get(), pointers, bytesNumber);
    }

    std::vector<BYTE> MemoryManagerExternal::ReadVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets, int bytesNumber) {
        return External::ReadVirtualMemory(_processHandle.get(), baseAddress, offsets, bytesNumber);
    }

    std::vector<BYTE> MemoryManagerExternal::ReadVirtualMemory(std::wstring moduleName, uintptr_t offset, size_t bytesNumber) {
        return External::ReadVirtualMemory(_processHandle.get(), moduleName, offset, bytesNumber);
    }

    std::vector<BYTE> MemoryManagerExternal::ReadVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, size_t bytesNumber) {
        return External::ReadVirtualMemory(_processHandle.get(), moduleName, offsets, bytesNumber);
    }

    template <typename TStructure>
    TStructure MemoryManagerExternal::ReadVirtualMemory(uintptr_t baseAddress) {
        return External::ReadVirtualMemory<TStructure>(_processHandle.get(), baseAddress);
    }

    template <typename TStructure>
    TStructure MemoryManagerExternal::ReadVirtualMemory(std::vector<uintptr_t> pointers) {
        return External::ReadVirtualMemory<TStructure>(_processHandle.get(), pointers);
    }

    template <typename TStructure>
    TStructure MemoryManagerExternal::ReadVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets) {
        return External::ReadVirtualMemory<TStructure>(_processHandle.get(), baseAddress, offsets);
    }

    template <typename TStructure>
    TStructure MemoryManagerExternal::ReadVirtualMemory(std::wstring moduleName, uintptr_t offset) {
        return External::ReadVirtualMemory<TStructure>(_processHandle.get(), moduleName, offset);
    }

    template <typename TStructure>
    TStructure MemoryManagerExternal::ReadVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets) {
        return External::ReadVirtualMemory<TStructure>(_processHandle.get(), moduleName, offsets);
    }

    void MemoryManagerExternal::WriteVirtualMemory(uintptr_t baseAddress, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle.get(), baseAddress, byteSequence);
    }

    void MemoryManagerExternal::WriteVirtualMemory(std::vector<uintptr_t> pointers, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle.get(), pointers, byteSequence);
    }

    void MemoryManagerExternal::WriteVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle.get(), baseAddress, offsets, byteSequence);
    }

    void MemoryManagerExternal::WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle.get(), moduleName, offset, byteSequence);
    }

    void MemoryManagerExternal::WriteVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, std::vector<byte> byteSequence) {
        External::WriteVirtualMemory(_processHandle.get(), moduleName, offsets, byteSequence);
    }

    template <typename TStructure>
    void MemoryManagerExternal::WriteVirtualMemory(uintptr_t baseAddress, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle.get(), baseAddress, structure);
    }

    template <typename TStructure>
    void MemoryManagerExternal::WriteVirtualMemory(std::vector<uintptr_t> pointers, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle.get(), pointers, structure);
    }

    template <typename TStructure>
    void MemoryManagerExternal::WriteVirtualMemory(uintptr_t baseAddress, std::vector<uintptr_t> pointers, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle.get(), baseAddress, pointers, structure);
    }

    template <typename TStructure>
    void MemoryManagerExternal::WriteVirtualMemory(std::wstring moduleName, uintptr_t offset, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle.get(), moduleName, offset, structure);
    }

    template <typename TStructure>
    void MemoryManagerExternal::WriteVirtualMemory(std::wstring moduleName, std::vector<uintptr_t> offsets, TStructure structure) {
        External::WriteVirtualMemory<TStructure>(_processHandle.get(), moduleName, offsets, structure);
    }

    uintptr_t MemoryManagerExternal::GetAddress(std::vector<uintptr_t> pointers) {
        return External::GetAddress(_processHandle.get(), pointers);
    }

    uintptr_t MemoryManagerExternal::GetAddress(uintptr_t baseAddress, std::vector<uintptr_t> pointers) {
        return External::GetAddress(_processHandle.get(), baseAddress, pointers);
    }

    uintptr_t MemoryManagerExternal::GetAddress(std::wstring moduleName, uintptr_t offset) {
        return External::GetAddress(_processHandle.get(), moduleName, offset);
    }

    uintptr_t MemoryManagerExternal::GetAddress(std::wstring moduleName, std::vector<uintptr_t> offsets) {
        return External::GetAddress(_processHandle.get(), moduleName, offsets);
    }
}
