#include "CodeInjector.h"

#include <utility>

namespace MemoryCommando {

    CodeInjector::CodeInjector(std::shared_ptr<const MemoryManager> memoryManager) :
        _pageSize(HelperMethods::GetSystemInfo().dwPageSize),
        _memoryManager{ std::move(memoryManager) } {
    }

    uintptr_t CodeInjector::InjectCode(const uintptr_t injectionAddress, const size_t originalInstructionLength, const std::vector<BYTE>& injectionMachineCode) const {
        // Allocate memory
        const size_t pageNumber = injectionMachineCode.size() / _pageSize + 1;
        const size_t allocationSize = pageNumber * _pageSize;
        const uintptr_t codeCaveAddress = _memoryManager->AllocateVirtualMemory(allocationSize);

        _memoryManager->WriteVirtualMemory(codeCaveAddress, injectionMachineCode);

        // Create injection trampoline
        std::vector<BYTE> injectionTrampolineMachineCode = GetTrampolineMachineCode(injectionAddress, codeCaveAddress);
        AddNops(injectionTrampolineMachineCode, originalInstructionLength);

        // Calculate trampoline back to the address after original jump
        const uintptr_t afterInjectionInstructionAddress = injectionAddress + originalInstructionLength;
        const uintptr_t afterCodeCaveAddress = codeCaveAddress + injectionMachineCode.size();
        const std::vector<BYTE> codeCaveTrampolineMachineCode = GetTrampolineMachineCode(afterCodeCaveAddress, afterInjectionInstructionAddress);

        // Write trampoline in the code cave back to the original code after original jump at the injection address
        _memoryManager->WriteVirtualMemory(afterCodeCaveAddress, codeCaveTrampolineMachineCode);

        // Write trampoline in the original code(at the injection address) to the code cave
        _memoryManager->WriteVirtualMemory(injectionAddress, injectionTrampolineMachineCode);

        return codeCaveAddress;
    }

    void CodeInjector::AddNops(std::vector<BYTE>& machineCode, const size_t neededLength) const {
        for(size_t length = machineCode.size(); length < neededLength; ++length) {
            machineCode.push_back(_nopCode);
        }
    }

    std::vector<BYTE> CodeInjector::GetTrampolineMachineCode(const uintptr_t originalAddress, const uintptr_t jumpAddress) const {
        std::vector<BYTE> injectionTrampolineMachineCode{ _relativeJumpCode };
        std::vector<BYTE> jumpOffsetBytes = HelperMethods::ConvertObjectToBytes(__int32(jumpAddress - (originalAddress + _relativeJumpSize)));
        injectionTrampolineMachineCode.insert(injectionTrampolineMachineCode.end(), jumpOffsetBytes.begin(), jumpOffsetBytes.end());

        return injectionTrampolineMachineCode;
    }

    void CodeInjector::AppendTrampolineMachineCode(std::vector<BYTE>& machineCode, uintptr_t originalAddress, uintptr_t jumpAddress) const {
        std::vector<BYTE> trampolineMachineCode = GetTrampolineMachineCode(originalAddress, jumpAddress);
        machineCode.insert(machineCode.end(), trampolineMachineCode.begin(), trampolineMachineCode.end());
    }


}
