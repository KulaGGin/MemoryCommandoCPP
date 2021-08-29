#pragma once
#include <memory>

#include "windows.h"
#include <TlHelp32.h>

#include <vector>
#include <string>

#include "Memory/Scan/ScanTypeDefinitions.h"

namespace MemoryCommando {
    using namespace Memory;
    class HelperMethods {
    public:
        static SYSTEM_INFO GetSystemInfo();
        static std::vector<PROCESSENTRY32W> GetRunningProcesses();
        static PROCESSENTRY32W GetProcess(DWORD processId);
        static PROCESSENTRY32W GetProcess(HANDLE processHandle);
        static PROCESSENTRY32W GetProcess(const std::wstring& processName, size_t processNumber = 1);

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

        template<typename Classname> static ByteSequence ConvertObjectToByteSequence(Classname object);
        template<typename Classname> static std::vector<BYTE> ConvertObjectToVectorOfBytes(Classname object);
        template<typename Classname> static Classname ConvertBytesToObject(const std::vector<BYTE>& bytes);

        static std::vector<std::string> SplitString(const std::string& string, char delimiter);
    };

    template <typename Classname>
    ByteSequence HelperMethods::ConvertObjectToByteSequence(Classname object) {
        const size_t objectSize = sizeof(Classname);
        ByteSequence objectByteSequence{};
        BYTE* objectBytePointer = reinterpret_cast<BYTE*>(&object);

        for(BYTE* bytePointer = objectBytePointer; bytePointer < objectBytePointer + objectSize; ++bytePointer) {
            objectByteSequence.push_back(*bytePointer);
        }

        return objectByteSequence;
    }

    template<typename Classname>
    std::vector<BYTE> HelperMethods::ConvertObjectToVectorOfBytes(Classname object) {
        const size_t objectSize = sizeof(Classname);

        std::vector<BYTE> objectByteSequence{};
        BYTE* objectBytePointer = reinterpret_cast<BYTE*>(&object);

        for(BYTE* bytePointer = objectBytePointer; bytePointer < objectBytePointer + objectSize; ++bytePointer) {
            objectByteSequence.push_back(*bytePointer);
        }

        return objectByteSequence;
    }

    template <typename Classname>
    Classname HelperMethods::ConvertBytesToObject(const std::vector<BYTE>& bytes) {
        auto objectPointer = std::make_unique<Classname>();

        std::memcpy(objectPointer.get(), &bytes[0], sizeof Classname);

        Classname object = *objectPointer;

        return object;
    }
}
