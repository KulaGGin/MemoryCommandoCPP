#pragma once
#include <activation.h>
#include <string>
#include <vector>

#include "HelperMethods.h"

namespace MemoryCommando::Memory {
    class MemoryScannerAbstract {
    public:
        virtual ~MemoryScannerAbstract() = default;
        virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::string& pattern) = 0;
        //virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const std::string& pattern) const = 0;
        //virtual std::vector<uintptr_t> ScanVirtualMemory(const std::string& pattern) const = 0;
        //virtual std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const std::string& pattern) const = 0;
        //virtual std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::string& pattern) const = 0;

        //virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const std::vector<BYTE>& bytePattern) const = 0;
        //virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const std::vector<BYTE>& bytePattern) const = 0;
        //virtual std::vector<uintptr_t> ScanVirtualMemory(const std::vector<BYTE>& bytePattern) const = 0;
        //virtual std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const std::vector<BYTE>& bytePattern) const = 0;
        //virtual std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::vector<BYTE>& bytePattern) const = 0;

        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const Classname& object) const;
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const Classname& object) const;
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const Classname& object) const;
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, Classname object) const;
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, Classname object) const;
    private:
        uintptr_t _minimumApplicationAddress{};
    };

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const Classname& object) const {
        auto objectBytesSequence = HelperMethods::ConvertObjectToBytes(object);
        auto scanResults = ScanVirtualMemory(scanStartAddress, scanEndAddress, objectBytesSequence);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t scanStartAddress, const Classname& object) const {
        auto objectByteSequence = HelperMethods::ConvertObjectToBytes(object);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, objectByteSequence);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const Classname& object) const {
        auto objectByteSequence = HelperMethods::ConvertObjectToBytes(object);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(_minimumApplicationAddress, objectByteSequence);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::wstring& moduleName, Classname object) const {
        auto objectByteSequence = HelperMethods::ConvertObjectToBytes(object);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(moduleName, objectByteSequence);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, Classname object) const {
        auto objectByteSequence = HelperMethods::ConvertObjectToBytes(object);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(moduleNames, objectByteSequence);

        return scanResults;
    }
}



