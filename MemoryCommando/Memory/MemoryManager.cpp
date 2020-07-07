#include "MemoryManager.h"

#include <boost/algorithm/string/predicate.hpp>
#include <stdexcept>

#include "../Exceptions/CreateToolhelp32SnapshotException.h"
#include "../Exceptions/GetProcessIdException.h"
#include "../Exceptions/Module32Exception.h"
#include "../Exceptions/OpenProcessException.h"
#include "../Exceptions/Process32Exception.h"

#include "Memory.h"

namespace MemoryCommando::Memory {
    PROCESSENTRY32W MemoryManager::GetProcess() {
        return Memory::GetProcess(_processId);
    }

    std::wstring MemoryManager::GetProcessName() {
            const PROCESSENTRY32W process = GetProcess();
            return process.szExeFile;
    }

    std::vector<MODULEENTRY32W> MemoryManager::GetModules() const {
        std::vector<MODULEENTRY32W> modules{};
        MODULEENTRY32               module{};
        module.dwSize = sizeof module ;

        const auto modulesSnapshot = wil::unique_tool_help_snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, _processId));

        if(!modulesSnapshot)
            throw Exceptions::CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot of modules.", GetLastError());

        bool copiedToBuffer = Module32First(modulesSnapshot.get(), &module);
        if(!copiedToBuffer)
            throw Exceptions::Module32Exception("Module32First failed to fill the buffer.", GetLastError());

        do {
            modules.push_back(module);
            copiedToBuffer = Module32Next(modulesSnapshot.get(), &module);
        } while(copiedToBuffer);

        if(GetLastError() != ERROR_NO_MORE_FILES)
            throw Exceptions::Module32Exception("Module32Next failed to fill the buffer.", GetLastError());

        return modules;
    }

    MODULEENTRY32W MemoryManager::GetModule(const std::wstring& moduleName) {
        std::vector<MODULEENTRY32W> modules = GetModules();

        for(auto currentModule : modules) {
            if(boost::algorithm::iequals(moduleName, currentModule.szModule))
                return currentModule;
        }

        throw std::runtime_error("Couldn't find a module with the specified name in the modules list.");
    }

    uintptr_t MemoryManager::GetModuleBaseAddress(const std::wstring& moduleName) {
        auto module = GetModule(moduleName);

        return uintptr_t(module.modBaseAddr);
    }

    size_t MemoryManager::GetModuleSize(const std::wstring& moduleName) {
        const auto module = GetModule(moduleName);
        return size_t(module.modBaseSize);
    }

    DWORD MemoryManager::GetProcessId() {
        return _processId;
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(const std::vector<uintptr_t>& pointers, const size_t bytesNumber) {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        std::vector<BYTE> byteSequence = ReadVirtualMemory(calculatedAddress, bytesNumber);

        return byteSequence;
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, const int bytesNumber) {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, offsets);
        std::vector<BYTE> byteSequence = ReadVirtualMemory(calculatedAddress, bytesNumber);

        return byteSequence;
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(const std::wstring& moduleName, const uintptr_t offset, const size_t bytesNumber) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        std::vector<BYTE> byteSequence = ReadVirtualMemory(calculatedAddress, bytesNumber);

        return byteSequence;
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const size_t bytesNumber) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        std::vector<BYTE> byteSequence = ReadVirtualMemory(calculatedAddress, bytesNumber);

        return byteSequence;
    }

    void MemoryManager::WriteVirtualMemory(const std::vector<uintptr_t>& pointers, const std::vector<byte>& byteSequence) {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        WriteVirtualMemory(calculatedAddress, byteSequence);
    }

    void MemoryManager::WriteVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, const std::vector<byte>& byteSequence) {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, offsets);
        WriteVirtualMemory(calculatedAddress, byteSequence);
    }

    void MemoryManager::WriteVirtualMemory(const std::wstring& moduleName, const uintptr_t offset, const std::vector<byte>& byteSequence) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        WriteVirtualMemory(calculatedAddress, byteSequence);
    }

    void MemoryManager::WriteVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const std::vector<byte>& byteSequence) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        WriteVirtualMemory(calculatedAddress, byteSequence);
    }

    uintptr_t MemoryManager::GetAddress(const std::vector<uintptr_t>& pointers) {
        auto pointerIterator = pointers.begin();
        uintptr_t baseAddress = *pointerIterator;
        ++pointerIterator;
        uintptr_t address{};
        while(pointerIterator < pointers.end() - 1) {
            address = MemoryManager::ReadVirtualMemory<uintptr_t>(baseAddress + *pointerIterator);
            baseAddress = address;

            ++pointerIterator;
        }

        const uintptr_t endAddress = address + *pointers.end() - 1;

        return endAddress;
    }

    uintptr_t MemoryManager::GetAddress(const uintptr_t baseAddress, std::vector<uintptr_t> offsets) {
        offsets.insert(offsets.begin(), baseAddress);
        const uintptr_t calculatedAddress = GetAddress(offsets);
        return calculatedAddress;
    }

    uintptr_t MemoryManager::GetAddress(const std::wstring& moduleName, const uintptr_t offset) {
        const uintptr_t baseAddress = GetModuleBaseAddress(moduleName);
        const uintptr_t calculatedAddress = baseAddress + offset;

        return calculatedAddress;
    }

    uintptr_t MemoryManager::GetAddress(const std::wstring& moduleName, std::vector<uintptr_t> offsets) {
        const uintptr_t baseAddress = GetModuleBaseAddress(moduleName);
        offsets.insert(offsets.begin(), baseAddress);
        const uintptr_t calculatedAddress = GetAddress(offsets);

        return calculatedAddress;
    }


    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const uintptr_t baseAddress) {
        std::vector<BYTE> byteSequence = ReadVirtualMemory(baseAddress, sizeof(TStructure));
        BYTE* bytePointer = &byteSequence[0];
        auto structure = reinterpret_cast<TStructure>(bytePointer);
        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const std::vector<uintptr_t>& pointers) {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& offsets) {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, offsets);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const std::wstring& moduleName, const uintptr_t offset) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const uintptr_t baseAddress, const TStructure& structure) {
        const size_t structureSize = sizeof(TStructure);
        std::vector<BYTE> structureByteSequence{};

        for(BYTE* bytePointer = &structure; bytePointer < &structure + structureSize; ++bytePointer) {
            structureByteSequence.push_back(*bytePointer);
        }

        WriteVirtualMemory(baseAddress, structureByteSequence);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const std::vector<uintptr_t>& pointers, const TStructure& structure) {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& pointers, const TStructure& structure) {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, pointers);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const std::wstring& moduleName, const uintptr_t offset, const TStructure& structure) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const TStructure& structure) {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    SYSTEM_INFO MemoryManager::GetSystemInfo() {
        SYSTEM_INFO systemInfo;
        ::GetSystemInfo(&systemInfo);
        return systemInfo;
    }

    std::vector<PROCESSENTRY32W> MemoryManager::GetRunningProcesses() {
        std::vector<PROCESSENTRY32W> runningProcesses{};
        PROCESSENTRY32 process{};
        process.dwSize = sizeof(process);

        const wil::unique_tool_help_snapshot processesSnapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

        if(!processesSnapshot)
            throw Exceptions::CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot.", GetLastError());

        bool copiedToBuffer = Process32First(processesSnapshot.get(), &process);
        if(!copiedToBuffer)
            throw Exceptions::Process32Exception("Process32First failed to fill the buffer.", GetLastError());

        do {
            runningProcesses.push_back(process);
            copiedToBuffer = Process32Next(processesSnapshot.get(), &process);
        } while(copiedToBuffer);

        if(!copiedToBuffer && GetLastError() != ERROR_NO_MORE_FILES)
            throw Exceptions::Process32Exception("Process32Next failed to fill the buffer.", GetLastError());

        return runningProcesses;
    }

    PROCESSENTRY32W MemoryManager::GetProcess(DWORD processId) {
        std::vector<PROCESSENTRY32W> processes = Memory::GetRunningProcesses();

        for(auto currentProcess : processes) {
            if(processId == currentProcess.th32ProcessID)
                return currentProcess;
        }

        throw std::runtime_error("Couldn't find a process with the specified processId in the process list.");
    }

    PROCESSENTRY32W MemoryManager::GetProcess(HANDLE processHandle) {
        const DWORD processId = GetProcessId(processHandle);
        const PROCESSENTRY32W process = GetProcess(processId);

        return process;
    }

    PROCESSENTRY32W MemoryManager::GetProcess(const std::wstring& processName, const size_t processNumber) {
        auto processes = Memory::GetRunningProcesses();

        size_t foundProcessNumber = 0;
        for(auto currentProcess : processes) {
            if(boost::iequals(processName, currentProcess.szExeFile)) {
                foundProcessNumber++;

                if(foundProcessNumber >= processNumber) {
                    return currentProcess;
                }
            }
        }

        throw std::runtime_error("Couldn't find a process with the specified process name in the process list.");
    }

    DWORD MemoryManager::GetProcessId(HANDLE processHandle) {
        const DWORD processId = ::GetProcessId(processHandle);

        if(!processId)
            throw Exceptions::GetProcessIdException("Couldn't get processId from the specified processHandle", GetLastError());

        return processId;
    }

    DWORD MemoryManager::GetProcessId(const std::wstring& processName, size_t processNumber) {
        const PROCESSENTRY32W process = GetProcess(processName, processNumber);
        const DWORD processId = process.th32ProcessID;

        return processId;
    }

    HANDLE MemoryManager::GetProcessHandle(DWORD processId, DWORD processAccess) {
        const HANDLE processHandle = ::OpenProcess(processAccess, 0, processId);

        if(!processHandle)
            throw Exceptions::OpenProcessException("OpenProcess couldn't get a handle to the specified processId with the error " + std::to_string(GetLastError()) + ".", GetLastError());

        return processHandle;
    }

    HANDLE MemoryManager::GetProcessHandle(const std::wstring& processName, size_t processNumber, DWORD processAccess) {
        const PROCESSENTRY32W process = GetProcess(processName, processNumber);
        const HANDLE processHandle = GetProcessHandle(process.th32ProcessID, processAccess);

        return processHandle;
    }

    std::wstring MemoryManager::GetProcessName(HANDLE processHandle) {
        const auto process = GetProcess(processHandle);
        const std::wstring processName = process.szExeFile;

        return processName;
    }

    std::wstring MemoryManager::GetProcessName(DWORD processId) {
        const auto process = GetProcess(processId);
        const std::wstring processName = process.szExeFile;

        return processName;
    }

    std::vector<MODULEENTRY32W> MemoryManager::GetModules(DWORD processId) {
        std::vector<MODULEENTRY32W> modules{};
        MODULEENTRY32               module{};
        module.dwSize = sizeof(module);

        const auto modulesSnapshot = wil::unique_tool_help_snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId));

        if(!modulesSnapshot)
            throw Exceptions::CreateToolhelp32SnapshotException("CreateToolhelp32Snapshot failed to create a snapshot of modules.", GetLastError());

        bool copiedToBuffer = Module32First(modulesSnapshot.get(), &module);
        if(!copiedToBuffer)
            throw Exceptions::Module32Exception("Module32First failed to fill the buffer.", GetLastError());

        do {
            modules.push_back(module);
            copiedToBuffer = Module32Next(modulesSnapshot.get(), &module);
        } while(copiedToBuffer);

        if(GetLastError() != ERROR_NO_MORE_FILES)
            throw Exceptions::Module32Exception("Module32Next failed to fill the buffer.", GetLastError());

        return modules;
    }

    std::vector<MODULEENTRY32W> MemoryManager::GetModules(HANDLE processHandle) {
        const auto processId = GetProcessId(processHandle);
        const auto modules = GetModules(processId);

        return modules;
    }

    std::vector<MODULEENTRY32W> MemoryManager::GetModules(const std::wstring& processName, size_t processNumber) {
        const auto processId = GetProcessId(processName, processNumber);
        const auto modules = GetModules(processId);

        return modules;
    }

    MODULEENTRY32W MemoryManager::GetModule(const std::wstring& moduleName, DWORD processId) {
        std::vector<MODULEENTRY32W> modules = GetModules(processId);

        for(auto currentModule : modules) {
            if(boost::algorithm::iequals(moduleName, currentModule.szModule))
                return currentModule;
        }

        throw std::runtime_error("Couldn't find a module with the specified name in the modules list.");
    }

    MODULEENTRY32W MemoryManager::GetModule(const std::wstring& moduleName, HANDLE processHandle) {
        const auto processId = GetProcessId(processHandle);
        const auto module = GetModule(moduleName, processId);

        return module;
    }

    MODULEENTRY32W MemoryManager::GetModule(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber) {
        const auto processId = GetProcessId(processName, processNumber);
        const auto module = GetModule(moduleName, processId);

        return module;
    }

    uintptr_t MemoryManager::GetModuleBaseAddress(const std::wstring& moduleName, DWORD processId) {
        auto module = GetModule(moduleName, processId);

        return uintptr_t(module.modBaseAddr);
    }

    uintptr_t MemoryManager::GetModuleBaseAddress(const std::wstring& moduleName, HANDLE processHandle) {
        const auto module = GetModule(moduleName, processHandle);

        return uintptr_t(module.modBaseAddr);
    }

    uintptr_t MemoryManager::GetModuleBaseAddress(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber) {
        const auto module = GetModule(moduleName, processName, processNumber);

        return uintptr_t(module.modBaseAddr);
    }

    size_t MemoryManager::GetModuleSize(const std::wstring& moduleName, DWORD processId) {
        const auto module = GetModule(moduleName, processId);
        return size_t(module.modBaseSize);
    }

    size_t MemoryManager::GetModuleSize(const std::wstring& moduleName, HANDLE processHandle) {
        const auto module = GetModule(moduleName, processHandle);
        return size_t(module.modBaseSize);
    }

    size_t MemoryManager::GetModuleSize(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber) {
        const auto module = GetModule(moduleName, processName, processNumber);
        return size_t(module.modBaseSize);
    }
}
