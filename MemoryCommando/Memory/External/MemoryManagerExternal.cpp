#include "MemoryManagerExternal.h"

#include <memory>
#include <stdexcept>

#include "../../Exceptions/OpenProcessException.h"
#include "../../Exceptions/ReadProcessMemoryException.h"
#include "../../Exceptions/VirtualAllocExException.h"
#include "../../Exceptions/VirtualFreeExException.h"
#include "../../Exceptions/VirtualProtectExException.h"
#include "../../Exceptions/VirtualQueryExException.h"
#include "../../Exceptions/WriteProcessMemoryException.h"
#include "../../HelperMethods.h"

namespace MemoryCommando::Memory::External {
    MemoryManagerExternal::MemoryManagerExternal(const DWORD processId, const DWORD processAccess) {
        _processId = processId;
        _process = MemoryManager::GetProcess();
        _processHandle = wil::unique_handle(GetNewProcessHandle(processAccess));
    }

    MemoryManagerExternal::MemoryManagerExternal(const std::wstring& processName, const size_t processNumber, const DWORD processAccess) {
        _process = HelperMethods::GetProcess(processName, processNumber);
        _processId = _process.th32ProcessID;
        _processHandle = wil::unique_handle(GetNewProcessHandle(processAccess));
    }

    DWORD MemoryManagerExternal::GetProcessId() const {
        return _process.th32ProcessID;
    }

    HANDLE MemoryManagerExternal::GetProcessHandle() const {
        return _processHandle.get();
    }

    HANDLE MemoryManagerExternal::GetNewProcessHandle(const DWORD processAccess) const {
        const HANDLE processHandle = OpenProcess(processAccess, 0, _processId);

        if(!processHandle)
            throw Exceptions::OpenProcessException("OpenProcess couldn't get a handle to the specified processId with the error " + std::to_string(GetLastError()) + ".", GetLastError());

        return processHandle;
    }

    uintptr_t MemoryManagerExternal::AllocateVirtualMemory(const uintptr_t baseAddress, const size_t allocationSize, const DWORD allocationType, const DWORD protectionType) const {
        LPVOID allocationAddress = VirtualAllocEx(_processHandle.get(), LPVOID(baseAddress), allocationSize, allocationType, protectionType);

        if(!allocationAddress) {
            throw Exceptions::VirtualAllocExException("VirtualAllocEx couldn't allocate memory with error code " + std::to_string(GetLastError()) + ".", GetLastError());
        }

        return uintptr_t(allocationAddress);
    }

    void MemoryManagerExternal::FreeVirtualMemory(const uintptr_t address, const DWORD freeType, const size_t size) const {
        if(freeType == MEM_RELEASE && size != 0)
            throw std::invalid_argument("When freeType is MEM_RELEASE, size must be 0.");

        const bool didFree = VirtualFreeEx(_processHandle.get(), LPVOID(address), size, freeType);

        if(!didFree)
            throw Exceptions::VirtualFreeExException("VirtualFreeEx couldn't free memory with error code " + std::to_string(GetLastError()) + ".", GetLastError());
    }

    void MemoryManagerExternal::ProtectVirtualMemory(const uintptr_t baseAddress, const size_t protectionSize, const DWORD protectionType) const {
        DWORD oldProtection;
        const bool didProtect = VirtualProtectEx(_processHandle.get(), LPVOID(baseAddress), protectionSize, protectionType, &oldProtection);

        if(!didProtect)
            throw Exceptions::VirtualProtectExException("VirtualProtectEx failed to protect memory with the error code " + std::to_string(GetLastError()) + ".", GetLastError());
    }

    MEMORY_BASIC_INFORMATION MemoryManagerExternal::QueryVirtualMemory(const uintptr_t baseAddress) const {
        MEMORY_BASIC_INFORMATION memoryBasicInformation{};

        const SIZE_T bytesReturned = VirtualQueryEx(_processHandle.get(), LPVOID(baseAddress), &memoryBasicInformation, sizeof memoryBasicInformation);

        if(!bytesReturned)
            throw Exceptions::VirtualQueryExException("VirtualQueryEx couldn't query memory and failed with error code " + std::to_string(GetLastError()), GetLastError());

        return memoryBasicInformation;
    }

    std::vector<BYTE> MemoryManagerExternal::ReadVirtualMemory(const uintptr_t address, const size_t bytesNumber) const {
        const std::unique_ptr<BYTE[]> byteBuffer(new BYTE[bytesNumber]);
        SIZE_T bytesReadNumber;
        const bool didReadMemory = ReadProcessMemory(_processHandle.get(), LPCVOID(address), byteBuffer.get(), bytesNumber, &bytesReadNumber);

        if(!didReadMemory)
            throw Exceptions::ReadProcessMemoryException("ReadProcessMemory couldn't read memory at address" + std::to_string(address) + " and failed with error code" + std::to_string(GetLastError()) + ".", GetLastError());

        const std::vector<BYTE> byteSequence(byteBuffer.get(), byteBuffer.get() + bytesReadNumber);

        return byteSequence;
    }

    void MemoryManagerExternal::WriteVirtualMemory(const uintptr_t address, const std::vector<byte>& byteSequence) const {
        const BYTE* firstBytePointer = &byteSequence[0];
        SIZE_T bytesWritten;

        const bool didWrite = WriteProcessMemory(_processHandle.get(), LPVOID(address), firstBytePointer, byteSequence.size(), &bytesWritten);

        if(!didWrite)
            throw Exceptions::WriteProcessMemoryException("WriteProcessMemory couldn't write memory and failed with the error " + std::to_string(GetLastError()) + ".", GetLastError());
        if(bytesWritten != byteSequence.size())
            throw Exceptions::WriteProcessMemoryException("WriteProcessMemory couldn't fully write byteSequence into the memory", GetLastError());
    }
}
