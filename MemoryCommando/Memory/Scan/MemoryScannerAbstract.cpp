#include "MemoryScannerAbstract.h"

namespace MemoryCommando::Memory {
    MemoryScannerAbstract::MemoryScannerAbstract() {
        _minimumApplicationAddress = uintptr_t(HelperMethods::GetSystemInfo().lpMinimumApplicationAddress);
        _maximumApplicationAddress = uintptr_t(HelperMethods::GetSystemInfo().lpMaximumApplicationAddress);
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t scanStartAddress, uintptr_t scanEndAddress, std::vector<BYTE> byteSequence) {
        BytePattern bytePattern{byteSequence};
        auto scanResults = ScanVirtualMemory(scanStartAddress, scanEndAddress, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(uintptr_t scanStartAddress, const std::vector<BYTE>& byteSequence) {
        BytePattern bytePattern{byteSequence};

        auto scanResults = ScanVirtualMemory(scanStartAddress, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::vector<BYTE>& byteSequence) {
        BytePattern bytePattern{byteSequence};

        auto scanResults = ScanVirtualMemory(bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::wstring& moduleName, const std::vector<BYTE>& byteSequence) {
        BytePattern bytePattern{byteSequence};

        auto scanResults = ScanVirtualMemory(moduleName, bytePattern);

        return scanResults;
    }

    std::vector<uintptr_t> MemoryScannerAbstract::ScanVirtualMemory(const std::vector<std::wstring>& moduleNames, const std::vector<BYTE>& byteSequence) {
        BytePattern bytePattern{byteSequence};

        auto scanResults = ScanVirtualMemory(moduleNames, bytePattern);

        return scanResults;
    }
}


