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
        _processId = GetCurrentProcessId();
        _processHandle = wil::unique_handle(GetCurrentProcess());
        _process = MemoryManager::GetProcess();
    }

    uintptr_t MemoryManagerInternal::AllocateVirtualMemory(const uintptr_t baseAddress, const size_t allocationSize, const DWORD allocationType, const DWORD protectionType) const {
        LPVOID allocationAddress = VirtualAlloc(reinterpret_cast<LPVOID>(baseAddress), allocationSize, allocationType, protectionType);

        if(!allocationAddress)
            throw Exceptions::VirtualAllocException("VirtualAlloc couldn't allocate memory with error code " + std::to_string(GetLastError()) + ".", GetLastError(), baseAddress, allocationSize, allocationType, protectionType);

        return reinterpret_cast<uintptr_t>(allocationAddress);
    }

    void MemoryManagerInternal::FreeVirtualMemory(const uintptr_t address, const DWORD freeType, const size_t size) const {
        if(freeType == MEM_RELEASE && size != 0)
            throw std::invalid_argument("When freeType is MEM_RELEASE, size must be 0.");

        const bool didFree = VirtualFree(reinterpret_cast<LPVOID>(address), size, freeType);

        if(!didFree) {
            std::string exceptionMessage = "VirtualFree couldn't free memory with error code " + std::to_string(GetLastError()) + ".";
            throw Exceptions::VirtualFreeException(exceptionMessage, GetLastError(), address, freeType, size);
        }
    }

    void MemoryManagerInternal::ProtectVirtualMemory(const uintptr_t address, const size_t protectionSize, const DWORD protectionType) const {
        DWORD oldProtection;

        const bool didProtect = VirtualProtect(reinterpret_cast<LPVOID>(address), protectionSize, protectionType, &oldProtection);

        if(!didProtect) {
            std::string exceptionMessage = "VirtualProtect failed to protect memory with the error code " + std::to_string(GetLastError()) + ".";
            throw Exceptions::VirtualProtectException(exceptionMessage, GetLastError(), address, protectionSize, protectionType);
        }
    }

    MEMORY_BASIC_INFORMATION MemoryManagerInternal::QueryVirtualMemory(const uintptr_t address) const {
        MEMORY_BASIC_INFORMATION memoryBasicInformation{};

        const SIZE_T bytesReturned = VirtualQuery(reinterpret_cast<LPVOID>(address), &memoryBasicInformation, sizeof memoryBasicInformation);

        if(!bytesReturned)
            throw Exceptions::VirtualQueryException("VirtualQuery couldn't query memory and failed with error code " + std::to_string(GetLastError()), GetLastError(), address);

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
