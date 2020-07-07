#pragma once
#include <string>
#include <vector>




#include "windows.h"
#include "TlHelp32.h"
#include <wil/resource.h>

namespace MemoryCommando::Memory {
    class MemoryManager {
    public:
        virtual ~MemoryManager() = default;

        virtual PROCESSENTRY32W GetProcess();
        virtual std::wstring GetProcessName();
        virtual std::vector<MODULEENTRY32W> GetModules() const;
        virtual MODULEENTRY32W GetModule(const std::wstring& moduleName);
        virtual uintptr_t GetModuleBaseAddress(const std::wstring& moduleName);
        virtual size_t GetModuleSize(const std::wstring& moduleName);

        virtual DWORD GetProcessId() = 0;
        virtual HANDLE GetProcessHandle() = 0;

        virtual uintptr_t AllocateVirtualMemory(uintptr_t baseAddress, size_t allocationSize, DWORD allocationType = MEM_RESERVE | MEM_COMMIT, DWORD protectionType = PAGE_EXECUTE_READWRITE) = 0;
        virtual void FreeVirtualMemory(uintptr_t baseAddress, DWORD freeType = MEM_RELEASE, size_t size = 0) = 0;
        virtual void ProtectVirtualMemory(uintptr_t baseAddress, size_t protectionSize, DWORD protectionType = PAGE_EXECUTE_READWRITE) = 0;
        virtual MEMORY_BASIC_INFORMATION QueryVirtualMemory(uintptr_t baseAddress) = 0;

        virtual std::vector<BYTE> ReadVirtualMemory(uintptr_t baseAddress, size_t bytesNumber) = 0;
        virtual std::vector<BYTE> ReadVirtualMemory(const std::vector<uintptr_t>& pointers, size_t bytesNumber);
        virtual std::vector<BYTE> ReadVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, int bytesNumber);
        virtual std::vector<BYTE> ReadVirtualMemory(const std::wstring& moduleName, uintptr_t offset, size_t bytesNumber);
        virtual std::vector<BYTE> ReadVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, size_t bytesNumber);
        // todo ReadVirtualMemory from array of bytes and offset

        virtual void WriteVirtualMemory(uintptr_t baseAddress, const std::vector<byte>& byteSequence) = 0;
        virtual void WriteVirtualMemory(const std::vector<uintptr_t>& pointers, const std::vector<byte>& byteSequence);
        virtual void WriteVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, const std::vector<byte>& byteSequence);
        virtual void WriteVirtualMemory(const std::wstring& moduleName, uintptr_t offset, const std::vector<byte>& byteSequence);
        virtual void WriteVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const std::vector<byte>& byteSequence);
        // todo WriteVirtualMemory from array of bytes and offset

        virtual uintptr_t GetAddress(const std::vector<uintptr_t>& pointers);
        virtual uintptr_t GetAddress(uintptr_t baseAddress, std::vector<uintptr_t> offsets);
        virtual uintptr_t GetAddress(const std::wstring& moduleName, uintptr_t offset);
        virtual uintptr_t GetAddress(const std::wstring& moduleName, std::vector<uintptr_t> offsets);
        // todo GetAddress from array of bytes and offset but add it to the MemoryCommander probably.

        template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress);
        template<typename TStructure> TStructure ReadVirtualMemory(const std::vector<uintptr_t>& pointers);
        template<typename TStructure> TStructure ReadVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets);
        template<typename TStructure> TStructure ReadVirtualMemory(const std::wstring& moduleName, uintptr_t offset);
        template<typename TStructure> TStructure ReadVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets);
        // todo ReadVirtualMemory from array of bytes and offset

        template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, const TStructure& structure);
        template<typename TStructure> void WriteVirtualMemory(const std::vector<uintptr_t>& pointers, const TStructure& structure);
        template<typename TStructure> void WriteVirtualMemory(uintptr_t baseAddress, const std::vector<uintptr_t>& pointers, const TStructure& structure);
        template<typename TStructure> void WriteVirtualMemory(const std::wstring& moduleName, uintptr_t offset, const TStructure& structure);
        template<typename TStructure> void WriteVirtualMemory(const std::wstring& moduleName, const std::vector<uintptr_t>& offsets, const TStructure& structure);
        // todo WriteVirtualMemory from array of bytes and offset

        // todo Commit, Reserve, Free memory functions that auto free memory on object destruction.

        static SYSTEM_INFO GetSystemInfo();
        static std::vector<PROCESSENTRY32W> GetRunningProcesses();
        static PROCESSENTRY32W GetProcess(DWORD processId);
        static PROCESSENTRY32W GetProcess(HANDLE processHandle);
        static PROCESSENTRY32W GetProcess(const std::wstring& processName, const size_t processNumber);

        static DWORD GetProcessId(HANDLE processHandle);
        static DWORD GetProcessId(const std::wstring& processName, size_t processNumber = 1);

        static HANDLE GetProcessHandle(DWORD processId, DWORD processAccess = PROCESS_ALL_ACCESS);
        static HANDLE GetProcessHandle(const std::wstring& processName, size_t processNumber = 1, DWORD processAccess = PROCESS_ALL_ACCESS);

        static std::wstring GetProcessName(HANDLE processHandle);
        static std::wstring GetProcessName(DWORD processId);

        static std::vector<MODULEENTRY32W> GetModules(DWORD processId);
        static std::vector<MODULEENTRY32W> GetModules(HANDLE processHandle);
        static std::vector<MODULEENTRY32W> GetModules(const std::wstring& processName, size_t processNumber = 1);

        static MODULEENTRY32W GetModule(const std::wstring& moduleName, DWORD processId);
        static MODULEENTRY32W GetModule(const std::wstring& moduleName, HANDLE processHandle);
        static MODULEENTRY32W GetModule(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber = 1);

        static uintptr_t GetModuleBaseAddress(const std::wstring& moduleName, DWORD processId);
        static uintptr_t GetModuleBaseAddress(const std::wstring& moduleName, HANDLE processHandle);
        static uintptr_t GetModuleBaseAddress(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber = 1);

        static size_t GetModuleSize(const std::wstring& moduleName, DWORD processId);
        static size_t GetModuleSize(const std::wstring& moduleName, HANDLE processHandle);
        static size_t GetModuleSize(const std::wstring& moduleName, const std::wstring& processName, size_t processNumber = 1);

    protected:
        wil::unique_handle _processHandle = nullptr;
        DWORD _processId = 0;
        PROCESSENTRY32W _process = {};
    };
}

