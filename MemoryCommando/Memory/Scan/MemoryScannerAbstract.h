#pragma once
#include <activation.h>
#include <string>
#include <vector>

#include "HelperMethods.h"
#include "Memory/Scan/BytePattern.h"

namespace MemoryCommando::Memory {
    class MemoryScannerAbstract {
    public:
        MemoryScannerAbstract();
        virtual ~MemoryScannerAbstract() = default;

        virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const std::string& stringPattern);
        virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t desiredStartAddress, const std::string& stringPattern);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::string& stringPattern);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const std::string& stringPattern);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::string& stringPattern);

        virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, std::vector<BYTE> byteSequence);
        virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const std::vector<BYTE>& byteSequence);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::vector<BYTE>& byteSequence);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const std::vector<BYTE>& byteSequence);
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::vector<BYTE>& byteSequence);

        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const Classname& object);
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const Classname& object);
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const Classname& object);
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, Classname object);
        template<typename Classname>
        std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, Classname object);
    protected:
        virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const BytePattern& bytePattern) = 0;
        virtual std::vector<uintptr_t> ScanVirtualMemory(uintptr_t scanStartAddress, const BytePattern& bytePattern) = 0;
        virtual std::vector<uintptr_t> ScanVirtualMemory(const BytePattern& bytePattern) = 0;
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::wstring& moduleName, const BytePattern& bytePattern) = 0;
        virtual std::vector<uintptr_t> ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const BytePattern& bytePattern) = 0;

        uintptr_t _minimumApplicationAddress{};
        uintptr_t _maximumApplicationAddress{};
    };

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, const Classname& object) {
        std::vector<BYTE> objectBytesSequence = HelperMethods::ConvertObjectToBytes(object);
        auto scanResults = ScanVirtualMemory(scanStartAddress, scanEndAddress, objectBytesSequence);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t scanStartAddress, const Classname& object) {
        auto objectByteSequence = HelperMethods::ConvertObjectToBytes(object);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(scanStartAddress, objectByteSequence);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const Classname& object) {
        auto objectByteSequence = HelperMethods::ConvertObjectToBytes(object);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(_minimumApplicationAddress, objectByteSequence);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::wstring& moduleName, Classname object) {
        auto objectByteSequence = HelperMethods::ConvertObjectToBytes(object);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(moduleName, objectByteSequence);

        return scanResults;
    }

    template <typename Classname>
    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, Classname object) {
        auto objectByteSequence = HelperMethods::ConvertObjectToBytes(object);

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(moduleNames, objectByteSequence);

        return scanResults;
    }
}



