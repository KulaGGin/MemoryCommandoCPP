#include "MemoryManagerInternal.h"
#include <processthreadsapi.h>
#include <stdexcept>



#include "../../Exceptions/CreateToolhelp32SnapshotException.h"
#include "../../Exceptions/Module32Exception.h"
#include "../../Exceptions/VirtualAllocException.h"
#include "../../Exceptions/VirtualFreeException.h"
#include "../../Exceptions/VirtualProtectException.h"
#include "../../Exceptions/VirtualQueryException.h"

namespace MemoryCommando::Memory::Internal {

    MemoryManagerInternal::MemoryManagerInternal() {
        _processHandle = wil::unique_handle(MemoryManagerInternal::GetProcessHandle());
        _processId = GetCurrentProcessId();
        _process = MemoryManager::GetProcess();
    }

    HANDLE MemoryManagerInternal::GetProcessHandle() const {
        return GetCurrentProcess();
    }

    uintptr_t MemoryManagerInternal::AllocateVirtualMemory(const uintptr_t baseAddress, const size_t allocationSize, const DWORD allocationType, const DWORD protectionType) const {
        LPVOID allocationAddress = VirtualAlloc(LPVOID(baseAddress), allocationSize, allocationType, protectionType);

        if(!allocationAddress)
            throw Exceptions::VirtualAllocException("VirtualAlloc couldn't allocate memory with error code " + std::to_string(GetLastError()) + ".", GetLastError());

        return uintptr_t(allocationAddress);
    }

    void MemoryManagerInternal::FreeVirtualMemory(const uintptr_t address, const DWORD freeType, const size_t size) const {
        if(freeType == MEM_RELEASE && size != 0)
            throw std::invalid_argument("When freeType is MEM_RELEASE, size must be 0.");

        const bool didFree = VirtualFree(LPVOID(address), size, freeType);

        if(!didFree)
            throw Exceptions::VirtualFreeException("VirtualFree couldn't free memory with error code " + std::to_string(GetLastError()) + ".", GetLastError());
    }

    void MemoryManagerInternal::ProtectVirtualMemory(const uintptr_t baseAddress, const size_t protectionSize, const DWORD protectionType) const {
        DWORD oldProtection;
        const bool didProtect = VirtualProtect(LPVOID(baseAddress), protectionSize, protectionType, &oldProtection);

        if(!didProtect)
            throw Exceptions::VirtualProtectException("VirtualProtect failed to protect memory with the error code " + std::to_string(GetLastError()) + ".", GetLastError());
    }

    MEMORY_BASIC_INFORMATION MemoryManagerInternal::QueryVirtualMemory(const uintptr_t baseAddress) const {
        MEMORY_BASIC_INFORMATION memoryBasicInformation{};

        const SIZE_T bytesReturned = VirtualQuery(LPVOID(baseAddress), &memoryBasicInformation, sizeof memoryBasicInformation);

        if(!bytesReturned)
            throw Exceptions::VirtualQueryException("VirtualQuery couldn't query memory and failed with error code " + std::to_string(GetLastError()), GetLastError());

        return memoryBasicInformation;
    }

    std::vector<BYTE> MemoryManagerInternal::ReadVirtualMemory(const uintptr_t address, const size_t bytesNumber) const {
        std::vector<BYTE> bytesSequence;
        bytesSequence.resize(bytesNumber, 0);
        std::memcpy(&bytesSequence[0], LPVOID(address), bytesNumber);

        return bytesSequence;
    }

    void MemoryManagerInternal::WriteVirtualMemory(const uintptr_t address, const std::vector<BYTE>& byteSequence) const {
        std::memcpy(LPVOID(address), &byteSequence[0], byteSequence.size());
    }
}
