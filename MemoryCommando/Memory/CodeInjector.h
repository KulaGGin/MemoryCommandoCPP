#pragma once

#include "Memory/MemoryManager.h"
#include <memory>

#include "HelperMethods.h"

namespace MemoryCommando {
    using namespace Memory;
    class CodeInjector {
    public:
        CodeInjector(std::shared_ptr<const MemoryManager> memoryManager);
        uintptr_t InjectCode(uintptr_t injectionAddress, size_t originalInstructionLength, const std::vector<BYTE>& injectionMachineCode) const;
        std::vector<BYTE> GetTrampolineMachineCode(uintptr_t originalAddress, uintptr_t jumpAddress) const;
        void AppendTrampolineMachineCode(std::vector<BYTE> &machineCode, uintptr_t originalAddress, uintptr_t jumpAddress) const;
    private:
        const size_t _relativeJumpSize = 5;
        const BYTE _relativeJumpCode = 0xE9;
        const BYTE _nopCode = 0x90;
        const size_t _pageSize;

        const std::shared_ptr<const MemoryManager> _memoryManager;

        void AddNops(std::vector<BYTE>& machineCode, size_t neededLength) const;
    };
}

