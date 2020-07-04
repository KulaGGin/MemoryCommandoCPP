#include "Internal.h"

#include "../Memory.h"
#include <memory>
#include <stdexcept>

#include "../../Exceptions/VirtualAllocException.h"
#include "../../Exceptions/VirtualFreeException.h"
#include "../../Exceptions/VirtualProtectException.h"
#include "../../Exceptions/VirtualQueryException.h"

namespace MemoryCommando::Memory::Internal {
    PROCESSENTRY32W GetProcess() {
        const DWORD currentProcessId = GetProcessId();
        return Memory::GetProcess(currentProcessId);
    }

    DWORD GetProcessId() {
        return GetCurrentProcessId();
    }

    HANDLE GetProcessHandle() {
        return GetCurrentProcess();
    }

    std::wstring GetProcessName() {
        const PROCESSENTRY32W currentProcess = GetProcess();
        return currentProcess.szExeFile;
    }

    std::vector<MODULEENTRY32W> GetModules() {
        const DWORD processId = GetProcessId();
        return Memory::GetModules(processId);
    }

    MODULEENTRY32W GetModule(const std::wstring& moduleName) {
        const DWORD currentProcessId = GetProcessId();
        return Memory::GetModule(moduleName, currentProcessId);
    }

    uintptr_t GetModuleBaseAddress(const std::wstring& moduleName) {
        const DWORD currentProcessId = GetProcessId();
        return Memory::GetModuleBaseAddress(moduleName, currentProcessId);
    }

    size_t GetModuleSize(const std::wstring& moduleName) {
        const DWORD currentProcessId = GetProcessId();
        return Memory::GetModuleSize(moduleName, currentProcessId);
    }

    uintptr_t AllocateVirtualMemory(const uintptr_t baseAddress, const size_t allocationSize, const DWORD allocationType, const DWORD protectionType) {
        LPVOID allocationAddress = VirtualAlloc(LPVOID(baseAddress), allocationSize, allocationType, protectionType);

        if(!allocationAddress)
            throw Exceptions::VirtualAllocException("VirtualAlloc couldn't allocate memory with error code " + std::to_string(GetLastError()) + ".", GetLastError());

        return uintptr_t(allocationAddress);
    }

    void FreeVirtualMemory(const uintptr_t address, const DWORD freeType, const size_t size) {
        if(freeType == MEM_RELEASE && size != 0)
            throw std::invalid_argument("When freeType is MEM_RELEASE, size must be 0.");

        const bool didFree = VirtualFree(LPVOID(address), size, freeType);

        if(!didFree)
            throw Exceptions::VirtualFreeException("VirtualFree couldn't free memory with error code " + std::to_string(GetLastError()) + ".", GetLastError());
    }

    void ProtectVirtualMemory(const uintptr_t baseAddress, const size_t protectionSize, const DWORD protectionType) {
        DWORD oldProtection;
        const bool didProtect = VirtualProtect(LPVOID(baseAddress), protectionSize, protectionType, &oldProtection);

        if(!didProtect)
            throw Exceptions::VirtualProtectException("VirtualProtect failed to protect memory with the error code " + std::to_string(GetLastError()) + ".", GetLastError());
    }

    MEMORY_BASIC_INFORMATION QueryVirtualMemory(const uintptr_t baseAddress) {
        MEMORY_BASIC_INFORMATION memoryBasicInformation{};

        const SIZE_T bytesReturned = VirtualQuery(LPVOID(baseAddress), &memoryBasicInformation, sizeof memoryBasicInformation);

        if(!bytesReturned)
            throw Exceptions::VirtualQueryException("VirtualQuery couldn't query memory and failed with error code " + std::to_string(GetLastError()), GetLastError());

        return memoryBasicInformation;
    }

    std::vector<BYTE> ReadVirtualMemory(const uintptr_t address, const int bytesNumber) {
        std::vector<BYTE> bytesSequence;
        bytesSequence.resize(bytesNumber, 0);
        std::memcpy(&bytesSequence[0], LPVOID(address), bytesNumber);

        return bytesSequence;
    }

    std::vector<BYTE> ReadVirtualMemory(const std::vector<uintptr_t>& pointers, const int bytesNumber) {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        std::vector<BYTE> byteSequence = ReadVirtualMemory(calculatedAddress, bytesNumber);

        return byteSequence;
    }

    std::vector<BYTE> ReadVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, const int bytesNumber) {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, offsets);
        std::vector<BYTE> byteSequence = ReadVirtualMemory(calculatedAddress, bytesNumber);

        return byteSequence;
    }

    std::vector<BYTE> ReadVirtualMemory(const std::wstring& moduleName, const uintptr_t offset, const int bytesNumber) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        std::vector<BYTE> byteSequence = ReadVirtualMemory(calculatedAddress, bytesNumber);

        return byteSequence;
    }

    std::vector<BYTE> ReadVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const int bytesNumber) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        std::vector<BYTE> byteSequence = ReadVirtualMemory(calculatedAddress, bytesNumber);

        return byteSequence;
    }

    template<typename TStructure> TStructure ReadVirtualMemory(const uintptr_t baseAddress) {
        std::vector<BYTE> byteSequence = ReadVirtualMemory(baseAddress, sizeof(TStructure));
        BYTE* bytePointer = &byteSequence[0];
        TStructure structure = reinterpret_cast<TStructure>(bytePointer);
        return structure;
    }

    template <typename TStructure>
    TStructure ReadVirtualMemory(const std::vector<uintptr_t> pointers) {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure ReadVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& offsets) {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, offsets);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure ReadVirtualMemory(const std::wstring& moduleName, const uintptr_t offset) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure ReadVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    void WriteVirtualMemory(const uintptr_t address, const std::vector<byte>& byteSequence) {
        std::memcpy(LPVOID(address), &byteSequence[0], byteSequence.size());
    }

    void WriteVirtualMemory(const std::vector<uintptr_t>& pointers, const std::vector<byte>& byteSequence) {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        WriteVirtualMemory(calculatedAddress, byteSequence);
    }

    void WriteVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, const std::vector<byte>& byteSequence) {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, offsets);
        WriteVirtualMemory(calculatedAddress, byteSequence);
    }

    void WriteVirtualMemory(const std::wstring& moduleName, const uintptr_t offset, const std::vector<byte>& byteSequence) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        WriteVirtualMemory(calculatedAddress, byteSequence);
    }

    void WriteVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const std::vector<byte>& byteSequence) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        WriteVirtualMemory(calculatedAddress, byteSequence);
    }


    template <typename TStructure>
    void WriteVirtualMemory(const uintptr_t baseAddress, const TStructure& structure) {
        const size_t structureSize = sizeof(TStructure);
        std::vector<BYTE> structureByteSequence{};

        for(BYTE* bytePointer = &structure; bytePointer < &structure + structureSize; ++bytePointer) {
            structureByteSequence.push_back(*bytePointer);
        }

        WriteVirtualMemory(baseAddress, structureByteSequence);
    }

    template <typename TStructure>
    void WriteVirtualMemory(const std::vector<uintptr_t>& pointers, const TStructure& structure) {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void WriteVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& pointers, const TStructure& structure) {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, pointers);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void WriteVirtualMemory(const std::wstring& moduleName, const uintptr_t offset, const TStructure& structure) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void WriteVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const TStructure& structure) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    uintptr_t GetAddress(const std::vector<uintptr_t>& pointers) {
        auto pointerIterator = pointers.begin();
        uintptr_t baseAddress = *pointerIterator;
        ++pointerIterator;
        uintptr_t address{};
        while(pointerIterator < pointers.end() - 1) {
            address = ReadVirtualMemory<uintptr_t>(baseAddress + *pointerIterator);
            baseAddress = address;

            ++pointerIterator;
        }

        const uintptr_t endAddress = address + *pointers.end() - 1;

        return endAddress;
    }

    uintptr_t GetAddress(const uintptr_t baseAddress, std::vector<uintptr_t> offsets) {
        offsets.insert(offsets.begin(), baseAddress);
        const uintptr_t calculatedAddress = GetAddress(offsets);
        return calculatedAddress;
    }

    uintptr_t GetAddress(const std::wstring& moduleName, const uintptr_t offset) {
        const uintptr_t baseAddress = GetModuleBaseAddress(moduleName);
        const uintptr_t calculatedAddress = baseAddress + offset;

        return calculatedAddress;
    }

    uintptr_t GetAddress(const std::wstring& moduleName, std::vector<uintptr_t> offsets) {
        const uintptr_t baseAddress = GetModuleBaseAddress(moduleName);
        offsets.insert(offsets.begin(), baseAddress);
        const uintptr_t calculatedAddress = GetAddress(offsets);

        return calculatedAddress;
    }

}
