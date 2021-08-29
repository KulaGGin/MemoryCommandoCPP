#include "MemoryScannerAbstract.h"

#include "Memory/Scan/PatternScanner.h"

namespace MemoryCommando::Memory {
    MemoryScannerAbstract::MemoryScannerAbstract() {
        _minimumApplicationAddress = uintptr_t(HelperMethods::GetSystemInfo().lpMinimumApplicationAddress);
        _maximumApplicationAddress = uintptr_t(HelperMethods::GetSystemInfo().lpMaximumApplicationAddress);
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t desiredStartAddress, uintptr_t desiredEndAddress, const std::string& stringPattern) {
        BytePattern bytePattern(stringPattern);

        ScanResults scanResults = ScanVirtualMemory(desiredStartAddress, desiredEndAddress, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t desiredStartAddress, const std::string& stringPattern) {
        BytePattern bytePattern{stringPattern};

        auto scanResults = ScanVirtualMemory(desiredStartAddress, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::string& stringPattern) {
        BytePattern bytePattern{stringPattern};

        auto scanResults = ScanVirtualMemory(bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::wstring& moduleName, const std::string& stringPattern) {
        BytePattern bytePattern{stringPattern};

        auto scanResults = ScanVirtualMemory(moduleName, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::string& stringPattern) {
        BytePattern bytePattern{stringPattern};

        std::vector<uintptr_t> scanResults = ScanVirtualMemory(moduleNames, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, ByteSequence byteSequence) {
        BytePattern bytePattern{byteSequence};
        auto scanResults = ScanVirtualMemory(scanStartAddress, scanEndAddress, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t scanStartAddress, const ByteSequence& byteSequence) {
        BytePattern bytePattern{byteSequence};

        auto scanResults = ScanVirtualMemory(scanStartAddress, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const ByteSequence& byteSequence) {
        BytePattern bytePattern{byteSequence};

        auto scanResults = ScanVirtualMemory(bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::wstring& moduleName, const ByteSequence& byteSequence) {
        BytePattern bytePattern{byteSequence};

        auto scanResults = ScanVirtualMemory(moduleName, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const ByteSequence& byteSequence) {
        BytePattern bytePattern{byteSequence};

        auto scanResults = ScanVirtualMemory(moduleNames, bytePattern);

        return scanResults;
    }
}


