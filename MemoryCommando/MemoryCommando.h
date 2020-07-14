#pragma once
#include <memory>
#include <windows.h>
#include <vector>

#include <TlHelp32.h>



#include "Memory/MemoryManager.h"
#include "Memory/Scan/MemoryScanner.h"


namespace MemoryCommando {
    using namespace Memory;
    class MemoryCommando {
    public:
        MemoryCommando();
        MemoryCommando(DWORD processId);
        MemoryCommando(const std::wstring& processName, size_t processNumber);

        PROCESSENTRY32W GetProcess() const;
        std::wstring GetProcessName() const;
        std::vector<MODULEENTRY32W> GetModules() const;
        MODULEENTRY32W GetModule(const std::wstring & moduleName) const;
        uintptr_t GetModuleBaseAddress(const std::wstring & moduleName) const;
        size_t GetModuleSize(const std::wstring & moduleName) const;

        DWORD GetProcessId() const;
        HANDLE GetProcessHandle() const;

        uintptr_t AllocateVirtualMemory(uintptr_t baseAddress, size_t allocationSize, DWORD allocationType = MEM_RESERVE | MEM_COMMIT, DWORD protectionType = PAGE_EXECUTE_READWRITE) const;
        void FreeVirtualMemory(uintptr_t baseAddress, DWORD freeType = MEM_RELEASE, size_t size = 0) const;
        void ProtectVirtualMemory(uintptr_t baseAddress, size_t protectionSize, DWORD protectionType = PAGE_EXECUTE_READWRITE) const;
        MEMORY_BASIC_INFORMATION QueryVirtualMemory(uintptr_t baseAddress) const;

        std::vector<BYTE> ReadVirtualMemory(uintptr_t baseAddress, size_t bytesNumber) const;
        std::vector<BYTE> ReadVirtualMemory(const std::vector<uintptr_t> & pointers, size_t bytesNumber) const;
        std::vector<BYTE> ReadVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t> & offsets, int bytesNumber) const;
        std::vector<BYTE> ReadVirtualMemory(const std::wstring & moduleName, uintptr_t offset, size_t bytesNumber) const;
        std::vector<BYTE> ReadVirtualMemory(const std::wstring & moduleName, const std::vector<uintptr_t> & offsets, size_t bytesNumber) const;
        // todo ReadVirtualMemory from array of bytes and offset

        void WriteVirtualMemory(uintptr_t baseAddress, const std::vector<byte> & byteSequence) const;
        void WriteVirtualMemory(const std::vector<uintptr_t> & pointers, const std::vector<byte> & byteSequence) const;
        void WriteVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t> & offsets, const std::vector<byte> & byteSequence) const;
        void WriteVirtualMemory(const std::wstring & moduleName, uintptr_t offset, const std::vector<byte> & byteSequence) const;
        void WriteVirtualMemory(const std::wstring & moduleName, const std::vector<uintptr_t> & offsets, const std::vector<byte> & byteSequence) const;
        // todo WriteVirtualMemory from array of bytes and offset

        uintptr_t GetAddress(const std::vector<uintptr_t> & pointers) const;
        uintptr_t GetAddress(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets) const;
        uintptr_t GetAddress(const std::wstring & moduleName, uintptr_t offset) const;
        uintptr_t GetAddress(const std::wstring& moduleName, std::vector<uintptr_t>& offsets) const;
        // todo GetAddress from array of bytes and offset but add it to the MemoryCommander probably.

        template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress) const;
        template<typename TStructure> TStructure ReadVirtualMemory(const std::vector<uintptr_t> & pointers) const;
        template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t> & offsets) const;
        template<typename TStructure> TStructure ReadVirtualMemory(const std::wstring & moduleName, uintptr_t offset) const;
        template<typename TStructure> TStructure ReadVirtualMemory(const std::wstring & moduleName, const std::vector<uintptr_t> & offsets) const;
        // todo ReadVirtualMemory from array of bytes and offset

        template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, const TStructure & structure) const;
        template<typename TStructure> void WriteVirtualMemory(const std::vector<uintptr_t> & pointers, const TStructure & structure) const;
        template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t> & offsets, const TStructure & structure) const;
        template<typename TStructure> void WriteVirtualMemory(const std::wstring & moduleName, uintptr_t offset, const TStructure & structure) const;
        template<typename TStructure> void WriteVirtualMemory(const std::wstring & moduleName, const std::vector<uintptr_t> & offsets, const TStructure & structure) const;
        // todo WriteVirtualMemory from array of bytes and offset

        // todo Commit, Reserve, Free memory functions that auto free memory on object destruction.

        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::string& bytePattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const std::string& bytePattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::string& bytePattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const std::string& bytePattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::string& bytePattern) const;

        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::vector<BYTE>& bytePattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const std::vector<BYTE>& bytePattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<BYTE>& bytePattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const std::vector<BYTE>& bytePattern) const;
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::vector<BYTE>& bytePattern) const;

        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const Classname& object) const;
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const Classname& object) const;
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const Classname& object) const;
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const Classname& object) const;
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const Classname& object) const;
    private:
        const std::shared_ptr<const MemoryManager> _memoryManager;
        const MemoryScanner &_memoryScanner;
    };
}