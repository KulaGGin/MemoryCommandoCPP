#include <windows.h>

#include "External.h"
#include <wil/resource.h>
#include <string>
#include <TlHelp32.h>
#include <boost/algorithm/string.hpp>
#include <string>


#include <wil/resource.h>
#include <string>
#include <TlHelp32.h>
#include <boost/algorithm/string.hpp>
#include <string>

#include <Psapi.h>
#include <boost/locale/encoding_utf.hpp>
#include <utility>
#include "CreateToolhelp32SnapshotException.h"
#include "GetProcessIdException.h"
#include "Module32Exception.h"
#include "Process32Exception.h"
#include "OpenProcessException.h"
#include "ReadProcessMemoryException.h"
#include "VirtualAllocExException.h"
#include "VirtualFreeExException.h"
#include "VirtualProtectExException.h"
#include "VirtualQueryExException.h"
#include "WriteProcessMemoryException.h"

#include "Memory.h"

namespace MemoryCommando::External {
    using namespace Exceptions;
    namespace conv = boost::locale::conv;
    namespace algorithm = boost::algorithm;
    namespace locale = boost::locale;

    PROCESSENTRY32W GetProcess(const DWORD processId) {
        return Memory::GetProcess(processId);
    }

    PROCESSENTRY32W GetProcess(HANDLE processHandle) {
        return Memory::GetProcess(processHandle);
    }

    PROCESSENTRY32W GetProcess(const std::wstring& processName, const size_t processNumber) {
        return Memory::GetProcess(processName, processNumber);
    }

    DWORD GetProcessId(const HANDLE processHandle) {
        const DWORD processId = ::GetProcessId(processHandle);

        if(!processId)
            throw GetProcessIdException("Couldn't get processId from the specified processHandle", GetLastError());

        return processId;
    }

    DWORD GetProcessId(const std::wstring& processName, const size_t processNumber) {
        const PROCESSENTRY32W process = GetProcess(processName, processNumber);
        const DWORD processId = process.th32ProcessID;

        return processId;
    }

    HANDLE GetProcessHandle(const DWORD processId, const DWORD processAccess) {
        const HANDLE processHandle = ::OpenProcess(processAccess, 0, processId);

        if (!processHandle)
            throw OpenProcessException("OpenProcess couldn't get a handle to the specified processId with the error " + std::to_string(GetLastError()) + ".", GetLastError());

        return processHandle;
    }

    HANDLE GetProcessHandle(const std::wstring& processName, const size_t processNumber, const DWORD processAccess) {
        const PROCESSENTRY32W process = GetProcess(processName, processNumber);
        const HANDLE processHandle = GetProcessHandle(process.th32ProcessID, processAccess);

        return processHandle;
    }

    std::wstring GetProcessName(const HANDLE processHandle) {
        const auto process = GetProcess(processHandle);
        const std::wstring processName = process.szExeFile;

        return processName;
    }

    std::wstring GetProcessName(const DWORD processId) {
        const auto process = GetProcess(processId);
        const std::wstring processName = process.szExeFile;

        return processName;
    }

    std::vector<MODULEENTRY32W> GetModules(const DWORD processId) {
        return Memory::GetModules(processId);
    }

    std::vector<MODULEENTRY32W> GetModules(HANDLE processHandle) {
        const auto processId = GetProcessId(processHandle);
        const auto modules = GetModules(processId);

        return modules;
    }

    std::vector<MODULEENTRY32W> GetModules(const std::wstring& processName, size_t processNumber) {
        const auto processId = GetProcessId(processName, processNumber);
        const auto modules = GetModules(processId);

        return modules;
    }

    MODULEENTRY32W GetModule(const std::wstring& moduleName, const DWORD processId) {
        return Memory::GetModule(moduleName, processId);
    }

    MODULEENTRY32W GetModule(const std::wstring& moduleName, HANDLE processHandle) {
        const auto processId = GetProcessId(processHandle);
        const auto module = GetModule(moduleName, processId);

        return module;
    }

    MODULEENTRY32W GetModule(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber) {
        const auto processId = GetProcessId(processName, processNumber);
        const auto module = GetModule(moduleName, processId);

        return module;
    }

    uintptr_t GetModuleBaseAddress(const std::wstring& moduleName, const DWORD processId) {
        return Memory::GetModuleBaseAddress(moduleName, processId);
    }

    uintptr_t GetModuleBaseAddress(const std::wstring& moduleName, HANDLE processHandle) {
        const auto module = GetModule(moduleName, processHandle);

        return uintptr_t(module.modBaseAddr);
    }

    uintptr_t GetModuleBaseAddress(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber) {
        const auto module = GetModule(moduleName,processName, processNumber);

        return uintptr_t(module.modBaseAddr);
    }

    size_t GetModuleSize(const DWORD processId, const std::wstring& moduleName) {
        return Memory::GetModuleSize(moduleName, processId);
    }

    size_t GetModuleSize(const std::wstring& moduleName, HANDLE processHandle) {
        const auto module = GetModule(moduleName, processHandle);
        return size_t(module.modBaseSize);
    }

    size_t GetModuleSize(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber) {
        const auto module = GetModule(moduleName, processName, processNumber);
        return size_t(module.modBaseSize);
    }

    uintptr_t AllocateVirtualMemory(const HANDLE processHandle, const uintptr_t baseAddress, const size_t allocationSize, const DWORD allocationType, const DWORD protectionType) {
        LPVOID allocationAddress = VirtualAllocEx(processHandle, LPVOID(baseAddress), allocationSize, allocationType, protectionType);

        if (!allocationAddress) {
            throw VirtualAllocExException("VirtualAllocEx couldn't allocate memory with error code " + std::to_string(GetLastError()) + ".", GetLastError());
        }

        return uintptr_t(allocationAddress);
    }

    void FreeVirtualMemory(const HANDLE processHandle, const uintptr_t address, const DWORD freeType, const size_t size) {
        if(freeType == MEM_RELEASE && size != 0)
            throw std::invalid_argument("When freeType is MEM_RELEASE, size must be 0.");

        const bool didFree = VirtualFreeEx(processHandle, LPVOID(address), size, freeType);

        if (!didFree)
            throw VirtualFreeExException("VirtualFreeEx couldn't free memory with error code " + std::to_string(GetLastError()) + ".", GetLastError());
    }

    void ProtectVirtualMemory(const HANDLE processHandle, const uintptr_t baseAddress, const size_t protectionSize, const DWORD protectionType) {
        DWORD oldProtection;
        const bool didProtect = VirtualProtectEx(processHandle, LPVOID(baseAddress), protectionSize, protectionType, &oldProtection);

        if (!didProtect)
            throw VirtualProtectExException("VirtualProtectEx failed to protect memory with the error code " + std::to_string(GetLastError()) + ".", GetLastError());
    }

    MEMORY_BASIC_INFORMATION QueryVirtualMemory(HANDLE processHandle, uintptr_t baseAddress) {
        MEMORY_BASIC_INFORMATION memoryBasicInformation{};

        const SIZE_T bytesReturned = VirtualQueryEx(processHandle, LPVOID(baseAddress), &memoryBasicInformation, sizeof memoryBasicInformation);

        if (!bytesReturned)
            throw VirtualQueryExException("VirtualQueryEx couldn't query memory and failed with error code " + std::to_string(GetLastError()), GetLastError());

        return memoryBasicInformation;
    }

    std::vector<BYTE> ReadVirtualMemory(HANDLE processHandle, uintptr_t address, int bytesNumber) {
        const std::unique_ptr<BYTE[]> byteBuffer(new BYTE[bytesNumber]);
        SIZE_T bytesReadNumber;
        const bool didReadMemory = ReadProcessMemory(processHandle, LPCVOID(address), byteBuffer.get(), bytesNumber, &bytesReadNumber);

        if (!didReadMemory)
            throw ReadProcessMemoryException("ReadProcessMemory couldn't read memory at address" + std::to_string(address) + " and failed with error code" + std::to_string(GetLastError()) + ".", GetLastError());

        const std::vector<BYTE> byteSequence(byteBuffer.get(), byteBuffer.get() + bytesReadNumber);

        return byteSequence;
    }

    std::vector<BYTE> ReadVirtualMemory(const HANDLE processHandle, std::vector<uintptr_t> pointers, const int bytesNumber) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, std::move(pointers));
        std::vector<BYTE> byteSequence = ReadVirtualMemory(processHandle, calculatedAddress, bytesNumber);

        return byteSequence;
    }

    std::vector<BYTE> ReadVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, std::vector<uintptr_t> offsets, int bytesNumber) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, baseAddress, offsets);
        std::vector<BYTE> byteSequence = ReadVirtualMemory(processHandle, calculatedAddress, bytesNumber);

        return byteSequence;
    }

    std::vector<BYTE> ReadVirtualMemory(const HANDLE processHandle, const std::wstring& moduleName, const uintptr_t offset, const int bytesNumber) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, moduleName, offset);
        std::vector<BYTE> byteSequence = ReadVirtualMemory(processHandle, calculatedAddress, bytesNumber);

        return byteSequence;
    }

    std::vector<BYTE> ReadVirtualMemory(HANDLE processHandle, std::wstring moduleName, std::vector<uintptr_t> offsets, int bytesNumber) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, std::move(moduleName), std::move(offsets));
        std::vector<BYTE> byteSequence = ReadVirtualMemory(processHandle, calculatedAddress, bytesNumber);

        return byteSequence;
    }

    template <typename TStructure>
    TStructure ReadVirtualMemory(HANDLE processHandle, uintptr_t baseAddress) {
        std::vector<BYTE> byteSequence = ReadVirtualMemory(processHandle, baseAddress, sizeof(TStructure));
        BYTE* bytePointer = &byteSequence[0];
        auto structure = reinterpret_cast<TStructure>(bytePointer);
        return structure;
    }

    template <typename TStructure>
    TStructure ReadVirtualMemory(HANDLE processHandle, std::vector<uintptr_t> pointers) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, pointers);
        TStructure structure = ReadVirtualMemory<TStructure>(processHandle, calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure ReadVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, std::vector<uintptr_t> offsets) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, baseAddress, offsets);
        TStructure structure = ReadVirtualMemory<TStructure>(processHandle, calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure ReadVirtualMemory(HANDLE processHandle, const std::wstring& moduleName, uintptr_t offset) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, moduleName, offset);
        TStructure structure = ReadVirtualMemory<TStructure>(processHandle, calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure ReadVirtualMemory(HANDLE processHandle, std::wstring moduleName, std::vector<uintptr_t> offsets) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, moduleName, offsets);
        TStructure structure = ReadVirtualMemory<TStructure>(processHandle, calculatedAddress);

        return structure;
    }

    void WriteVirtualMemory(HANDLE processHandle, uintptr_t address, const std::vector<byte>& byteSequence) {
        const BYTE* firstBytePointer = &byteSequence[0];
        SIZE_T bytesWritten;

        const bool didWrite = WriteProcessMemory(processHandle, LPVOID(address), firstBytePointer, byteSequence.size(), &bytesWritten);

        if(!didWrite)
            throw WriteProcessMemoryException("WriteProcessMemory couldn't write memory and failed with the error " + std::to_string(GetLastError()) + ".", GetLastError());
        if (bytesWritten != byteSequence.size())
            throw WriteProcessMemoryException("WriteProcessMemory couldn't fully write byteSequence into the memory", GetLastError());
    }

    void WriteVirtualMemory(HANDLE processHandle, std::vector<uintptr_t> pointers, std::vector<byte> byteSequence) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, pointers);
        WriteVirtualMemory(processHandle, calculatedAddress, byteSequence);
    }

    void WriteVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, std::vector<uintptr_t> offsets, std::vector<byte> byteSequence) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, baseAddress, offsets);
        WriteVirtualMemory(processHandle, calculatedAddress, byteSequence);
    }

    void WriteVirtualMemory(HANDLE processHandle, std::wstring moduleName, uintptr_t offset, std::vector<byte> byteSequence) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, moduleName, offset);
        WriteVirtualMemory(processHandle, calculatedAddress, byteSequence);
    }

    void WriteVirtualMemory(HANDLE processHandle, const std::wstring& moduleName, std::vector<uintptr_t> offsets, std::vector<byte> byteSequence) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, moduleName, offsets);
        WriteVirtualMemory(processHandle, calculatedAddress, byteSequence);
    }

    template <typename TStructure>
    void WriteVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, TStructure structure) {
        const size_t structureSize = sizeof(TStructure);
        std::vector<BYTE> structureByteSequence{};

        for(BYTE* bytePointer = &structure; bytePointer < &structure + structureSize; ++bytePointer) {
            structureByteSequence.push_back(*bytePointer);
        }

        WriteVirtualMemory(processHandle, baseAddress, structureByteSequence);
    }

    template <typename TStructure>
    void WriteVirtualMemory(HANDLE processHandle, std::vector<uintptr_t> pointers, TStructure structure) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, pointers);
        WriteVirtualMemory(processHandle, calculatedAddress, structure);
    }

    template <typename TStructure>
    void WriteVirtualMemory(HANDLE processHandle, uintptr_t baseAddress, std::vector<uintptr_t> pointers, TStructure structure) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, baseAddress, pointers);
        WriteVirtualMemory(processHandle, calculatedAddress, structure);
    }

    template <typename TStructure>
    void WriteVirtualMemory(HANDLE processHandle, std::wstring moduleName, uintptr_t offset, TStructure structure) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, moduleName, offset);
        WriteVirtualMemory(processHandle, calculatedAddress, structure);
    }

    template <typename TStructure>
    void WriteVirtualMemory(HANDLE processHandle, std::wstring moduleName, std::vector<uintptr_t> offsets, TStructure structure) {
        const uintptr_t calculatedAddress = GetAddress(processHandle, moduleName, offsets);
        WriteVirtualMemory(processHandle, calculatedAddress, structure);
    }

    uintptr_t GetAddress(HANDLE processHandle, std::vector<uintptr_t> pointers) {
        auto pointerIterator = pointers.begin();
        uintptr_t baseAddress = *pointerIterator;
        ++pointerIterator;
        uintptr_t address{};
        while (pointerIterator < pointers.end() - 1) {
            address = ReadVirtualMemory<uintptr_t>(processHandle, baseAddress + *pointerIterator);
            baseAddress = address;

            ++pointerIterator;
        }

        const uintptr_t endAddress = address + *pointers.end() - 1;

        return endAddress;
    }

    uintptr_t GetAddress(HANDLE processHandle, uintptr_t baseAddress, std::vector<uintptr_t> offsets) {
        offsets.insert(offsets.begin(), baseAddress);
        const uintptr_t calculatedAddress = GetAddress(processHandle, offsets);
        return calculatedAddress;
    }

    uintptr_t GetAddress(HANDLE processHandle, const std::wstring& moduleName, uintptr_t offset) {
        const uintptr_t baseAddress = GetModuleBaseAddress(moduleName, processHandle);
        const uintptr_t calculatedAddress = baseAddress + offset;

        return calculatedAddress;
    }

    uintptr_t GetAddress(HANDLE processHandle, const std::wstring& moduleName, std::vector<uintptr_t> offsets) {
        const uintptr_t baseAddress = GetModuleBaseAddress(moduleName, processHandle);
        offsets.insert(offsets.begin(), baseAddress);
        const uintptr_t calculatedAddress = GetAddress(processHandle, offsets);

        return calculatedAddress;
    }
}
