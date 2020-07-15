#include "CodeInjector.h"

#include <utility>

namespace MemoryCommando {

    CodeInjector::CodeInjector(std::shared_ptr<const MemoryManager> memoryManager) :
        _pageSize(HelperMethods::GetSystemInfo().dwPageSize),
        _memoryManager{ std::move(memoryManager) } {
    }

    void CodeInjector::InjectCode(const uintptr_t injectionAddress, const size_t instructionLength, const std::vector<BYTE>& machineCode) const {
        // Allocate memory
        const size_t pageNumber = machineCode.size() / _pageSize + 1;
        const size_t allocationSize = pageNumber * _pageSize;
        const uintptr_t codeCaveAddress = _memoryManager->AllocateVirtualMemory(allocationSize);

        _memoryManager->WriteVirtualMemory(codeCaveAddress, machineCode);

        // Create injection trampoline
        std::vector<BYTE> injectionTrampolineMachineCode = GetTrampolineMachineCode(injectionAddress, codeCaveAddress);
        AddNops(injectionTrampolineMachineCode, instructionLength);

        // Calculate trampoline back to the address after original jump
        const uintptr_t afterInjectionInstructionAddress = injectionAddress + instructionLength;
        const uintptr_t afterCodeCaveAddress = codeCaveAddress + machineCode.size();
        const std::vector<BYTE> codeCaveTrampolineMachineCode = GetTrampolineMachineCode(afterCodeCaveAddress, afterInjectionInstructionAddress);

        // Write trampoline in the code cave back to the original code after original jump at the injection address
        _memoryManager->WriteVirtualMemory(afterCodeCaveAddress, codeCaveTrampolineMachineCode);

        // Write trampoline in the original code(at the injection address) to the code cave
        _memoryManager->WriteVirtualMemory(injectionAddress, injectionTrampolineMachineCode);
    }

    void CodeInjector::AddNops(std::vector<BYTE>& machineCode, const size_t neededLength) const {
        for(size_t length = machineCode.size(); length < neededLength; ++length) {
            machineCode.push_back(_nopCode);
        }
    }

    std::vector<BYTE> CodeInjector::GetTrampolineMachineCode(const uintptr_t originalAddress, const uintptr_t jumpAddress) const {
        std::vector<BYTE> injectionTrampolineMachineCode{ _relativeJumpCode };
        std::vector<BYTE> jumpOffsetBytes = HelperMethods::ConvertObjectToBytes(ptrdiff_t(jumpAddress - (originalAddress + _relativeJumpSize)));
        injectionTrampolineMachineCode.insert(injectionTrampolineMachineCode.end(), jumpOffsetBytes.begin(), jumpOffsetBytes.end());

        return injectionTrampolineMachineCode;
    }
}
