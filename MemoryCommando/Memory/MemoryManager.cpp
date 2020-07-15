#include "MemoryManager.h"

#include <boost/algorithm/string/predicate.hpp>
#include <stdexcept>

#include "../Exceptions/CreateToolhelp32SnapshotException.h"
#include "../Exceptions/Module32Exception.h"
#include "../HelperMethods.h"

namespace MemoryCommando::Memory {
    PROCESSENTRY32W MemoryManager::GetProcess() const {
        return HelperMethods::GetProcess(_processId);
    }

    std::wstring MemoryManager::GetProcessName() const {
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

    MODULEENTRY32W MemoryManager::GetModule(const std::wstring& moduleName) const {
        std::vector<MODULEENTRY32W> modules = GetModules();

        for(auto currentModule : modules) {
            if(boost::algorithm::iequals(moduleName, currentModule.szModule))
                return currentModule;
        }

        throw std::runtime_error("Couldn't find a module with the specified name in the modules list.");
    }

    uintptr_t MemoryManager::GetModuleBaseAddress(const std::wstring& moduleName) const {
        auto module = GetModule(moduleName);

        return uintptr_t(module.modBaseAddr);
    }

    size_t MemoryManager::GetModuleSize(const std::wstring& moduleName) const {
        const auto module = GetModule(moduleName);
        return size_t(module.modBaseSize);
    }

    DWORD MemoryManager::GetProcessId() const {
        return _processId;
    }

    uintptr_t MemoryManager::AllocateVirtualMemory(const uintptr_t allocationSize) const {
        const uintptr_t allocatedAddress = AllocateVirtualMemory(0, allocationSize);

        return allocatedAddress;
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(const std::vector<uintptr_t>& pointers, const size_t bytesNumber) const {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        std::vector<BYTE> byteSequence = ReadVirtualMemory(calculatedAddress, bytesNumber);

        return byteSequence;
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, const int bytesNumber) const {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, offsets);
        std::vector<BYTE> byteSequence = ReadVirtualMemory(calculatedAddress, bytesNumber);

        return byteSequence;
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(const std::wstring& moduleName, const uintptr_t offset, const size_t bytesNumber) const {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        std::vector<BYTE> byteSequence = ReadVirtualMemory(calculatedAddress, bytesNumber);

        return byteSequence;
    }

    std::vector<BYTE> MemoryManager::ReadVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const size_t bytesNumber) const {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        std::vector<BYTE> byteSequence = ReadVirtualMemory(calculatedAddress, bytesNumber);

        return byteSequence;
    }

    void MemoryManager::WriteVirtualMemory(const std::vector<uintptr_t>& pointers, const std::vector<BYTE>& byteSequence) const {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        WriteVirtualMemory(calculatedAddress, byteSequence);
    }

    void MemoryManager::WriteVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, const std::vector<BYTE>& byteSequence) const {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, offsets);
        WriteVirtualMemory(calculatedAddress, byteSequence);
    }

    void MemoryManager::WriteVirtualMemory(const std::wstring& moduleName, const uintptr_t offset, const std::vector<BYTE>& byteSequence) const {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        WriteVirtualMemory(calculatedAddress, byteSequence);
    }

    void MemoryManager::WriteVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const std::vector<BYTE>& byteSequence) const {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        WriteVirtualMemory(calculatedAddress, byteSequence);
    }

    uintptr_t MemoryManager::GetAddress(const std::vector<uintptr_t>& pointers) const {
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

    uintptr_t MemoryManager::GetAddress(const uintptr_t baseAddress, std::vector<uintptr_t> offsets) const {
        offsets.insert(offsets.begin(), baseAddress);
        const uintptr_t calculatedAddress = GetAddress(offsets);
        return calculatedAddress;
    }

    uintptr_t MemoryManager::GetAddress(const std::wstring& moduleName, const uintptr_t offset) const {
        const uintptr_t baseAddress = GetModuleBaseAddress(moduleName);
        const uintptr_t calculatedAddress = baseAddress + offset;

        return calculatedAddress;
    }

    uintptr_t MemoryManager::GetAddress(const std::wstring& moduleName, std::vector<uintptr_t> offsets) const {
        const uintptr_t baseAddress = GetModuleBaseAddress(moduleName);
        offsets.insert(offsets.begin(), baseAddress);
        const uintptr_t calculatedAddress = GetAddress(offsets);

        return calculatedAddress;
    }


    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const uintptr_t baseAddress) const {
        std::vector<BYTE> byteSequence = ReadVirtualMemory(baseAddress, sizeof(TStructure));
        BYTE* bytePointer = &byteSequence[0];
        auto structure = reinterpret_cast<TStructure>(bytePointer);
        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const std::vector<uintptr_t>& pointers) const {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& offsets) const {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, offsets);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const std::wstring& moduleName, const uintptr_t offset) const {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    TStructure MemoryManager::ReadVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets) const {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        TStructure structure = ReadVirtualMemory<TStructure>(calculatedAddress);

        return structure;
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const uintptr_t baseAddress, const TStructure& structure) const {
        const size_t structureSize = sizeof(TStructure);
        std::vector<BYTE> structureByteSequence{};

        for(BYTE* bytePointer = &structure; bytePointer < &structure + structureSize; ++bytePointer) {
            structureByteSequence.push_back(*bytePointer);
        }

        WriteVirtualMemory(baseAddress, structureByteSequence);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const std::vector<uintptr_t>& pointers, const TStructure& structure) const {
        const uintptr_t calculatedAddress = GetAddress(pointers);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const uintptr_t baseAddress, const std::vector<uintptr_t>& pointers, const TStructure& structure) const {
        const uintptr_t calculatedAddress = GetAddress(baseAddress, pointers);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const std::wstring& moduleName, const uintptr_t offset, const TStructure& structure) const {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offset);
        WriteVirtualMemory(calculatedAddress, structure);
    }

    template <typename TStructure>
    void MemoryManager::WriteVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const TStructure& structure) const {
        const uintptr_t calculatedAddress = GetAddress(moduleName, offsets);
        WriteVirtualMemory(calculatedAddress, structure);
    }
}
