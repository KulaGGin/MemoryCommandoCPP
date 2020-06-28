#include "External.h"

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

namespace MemoryCommando::External {
    using namespace Exceptions;
    namespace conv = boost::locale::conv;
    namespace algorithm = boost::algorithm;
    namespace locale = boost::locale;

    PROCESSENTRY32W GetProcess(const DWORD processId) {
        std::vector<PROCESSENTRY32W> processes = GetRunningProcesses();

        for (auto currentProcess : processes) {
            if (processId == currentProcess.th32ProcessID)
                return currentProcess;
        }

        throw std::runtime_error("Couldn't find a process with the specified processId in the process list.");
    }

    PROCESSENTRY32W GetProcess(const std::string& processName, const size_t processNumber) {
        const std::wstring processNameWide = boost::locale::conv::utf_to_utf<WCHAR>(processName);
        const PROCESSENTRY32W wantedProcess = GetProcess(processNameWide, processNumber);
        return wantedProcess;
    }

    PROCESSENTRY32W GetProcess(const std::wstring& processName, const size_t processNumber) {
        auto processes = GetRunningProcesses();

        size_t foundProcessNumber = 0;
        for (auto currentProcess : processes) {
            if (boost::iequals(processName, currentProcess.szExeFile)) {
                foundProcessNumber++;

                if (foundProcessNumber >= processNumber) {
                    return currentProcess;
                }
            }
        }

        throw std::runtime_error("Couldn't find a process with the specified process name in the process list.");
    }

    DWORD GetProcessId(const HANDLE processHandle) {
        const DWORD processId = ::GetProcessId(processHandle);

        if(!processId)
            throw GetProcessIdException("Couldn't get processId from the specified processHandle", GetLastError());

        return processId;
    }

    DWORD GetProcessId(const std::wstring& processName, const size_t processNumber) {
        const auto process = GetProcess(processName, processNumber);
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
        const auto processName = GetProcessName(GetProcessId(processHandle));

        return processName;
    }

    std::wstring GetProcessName(const DWORD processId) {
        const auto process = GetProcess(processId);
        const std::wstring processName = process.szExeFile;

        return processName;
    }

    std::vector<PROCESSENTRY32W> GetRunningProcesses() {
        std::vector<PROCESSENTRY32W> runningProcesses{};
        PROCESSENTRY32 process{};
        process.dwSize = sizeof(process);

        const wil::unique_tool_help_snapshot processesSnapshot{CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)};

        if(!processesSnapshot)
            throw CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot.", GetLastError());

        bool copiedToBuffer = Process32First(processesSnapshot.get(), &process);
        if(!copiedToBuffer)
            throw Process32Exception("Process32First failed to fill the buffer.", GetLastError());

        do {
            runningProcesses.push_back(process);
            copiedToBuffer = Process32Next(processesSnapshot.get(), &process);
        } while (copiedToBuffer);

        if(!copiedToBuffer && GetLastError() != ERROR_NO_MORE_FILES)
            throw Process32Exception("Process32Next failed to fill the buffer.", GetLastError());

        return runningProcesses;
    }

    std::vector<MODULEENTRY32W> GetModules(const DWORD processId) {
        std::vector<MODULEENTRY32W> modules{};
        MODULEENTRY32               module{};
        module.dwSize = sizeof(module);

        const auto modulesSnapshot = wil::unique_tool_help_snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId));

        if (!modulesSnapshot)
            throw CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot of modules.", GetLastError());

        bool copiedToBuffer = Module32First(modulesSnapshot.get(), &module);
        if (!copiedToBuffer)
            throw Module32Exception("Module32First failed to fill the buffer.", GetLastError());

        do {
            modules.push_back(module);
            copiedToBuffer = Module32Next(modulesSnapshot.get(), &module);
        } while (copiedToBuffer);

        if (GetLastError() != ERROR_NO_MORE_FILES)
            throw Module32Exception("Module32Next failed to fill the buffer.", GetLastError());

        return modules;
    }

    MODULEENTRY32W GetModule(const DWORD processId, const std::wstring& moduleName) {
        std::vector<MODULEENTRY32W> modules = GetModules(processId);

        for (auto currentModule : modules) {
            if (algorithm::iequals(moduleName, currentModule.szModule))
                return currentModule;
        }

        throw std::runtime_error("Couldn't find a module with the specified name in the modules list.");
    }

    uintptr_t GetModuleBaseAddress(const DWORD processId, const std::wstring& moduleName) {
        auto module = GetModule(processId, moduleName);
        return uintptr_t(module.modBaseAddr);
    }

    size_t GetModuleSize(const DWORD processId, const std::wstring& moduleName) {
        const auto module = GetModule(processId, moduleName);
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
        const DWORD processId = GetProcessId(processHandle);
        const uintptr_t baseAddress = GetModuleBaseAddress(processId, moduleName);
        const uintptr_t calculatedAddress = baseAddress + offset;

        return calculatedAddress;
    }

    uintptr_t GetAddress(HANDLE processHandle, const std::wstring& moduleName, std::vector<uintptr_t> offsets) {
        const DWORD processId = GetProcessId(processHandle);
        const uintptr_t baseAddress = GetModuleBaseAddress(processId, moduleName);
        offsets.insert(offsets.begin(), baseAddress);
        const uintptr_t calculatedAddress = GetAddress(processHandle, offsets);

        return calculatedAddress;
    }
}
