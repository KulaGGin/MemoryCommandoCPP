#include "DetourCreator.h"

#include <utility>

namespace MemoryCommando {

    DetourCreator::DetourCreator(std::shared_ptr<const MemoryManager> memoryManager) :
        _pageSize(HelperMethods::GetSystemInfo().dwPageSize),
        _memoryManager{ std::move(memoryManager) } {
    }

    void DetourCreator::InjectCode(const uintptr_t injectionAddress, const size_t instructionLength, const std::vector<BYTE>& machineCode) const {
        // Allocate memory
        const size_t pageNumber = machineCode.size() / _pageSize + 1;
        const size_t allocationSize = pageNumber * _pageSize;
        const uintptr_t codeCaveAddress = _memoryManager->AllocateVirtualMemory(allocationSize);

        _memoryManager->WriteVirtualMemory(codeCaveAddress, machineCode);

        // Create injection trampoline
        std::vector<BYTE> injectionTrampolineMachineCode{ _relativeJumpCode };
        std::vector<BYTE> jumpOffsetBytes = HelperMethods::ConvertObjectToBytes(ptrdiff_t(codeCaveAddress - injectionAddress + _relativeJumpSize));
        injectionTrampolineMachineCode.insert(injectionTrampolineMachineCode.end(), jumpOffsetBytes.begin(), jumpOffsetBytes.end());
        AddNops(injectionTrampolineMachineCode, instructionLength);

        // Calculate trampoline back to the address after original jump
        std::vector<BYTE> codeCaveTrampolineMachineCode{ _relativeJumpCode };
        const uintptr_t afterInjectionInstructionAddress = injectionAddress + instructionLength;
        const uintptr_t afterCodeCaveAddress = codeCaveAddress + machineCode.size();
        std::vector<BYTE> codeCaveTrampolineOffsetBytes = HelperMethods::ConvertObjectToBytes(ptrdiff_t(afterInjectionInstructionAddress - afterCodeCaveAddress + _relativeJumpSize));
        codeCaveTrampolineMachineCode.insert(codeCaveTrampolineMachineCode.end(), codeCaveTrampolineOffsetBytes.begin(), codeCaveTrampolineOffsetBytes.end());

        // Write trampoline in the code cave back to the original code after original jump at the injection address
        _memoryManager->WriteVirtualMemory(afterCodeCaveAddress, codeCaveTrampolineMachineCode);

        // Write trampoline in the original code(at the injection address) to the code cave
        _memoryManager->WriteVirtualMemory(injectionAddress, injectionTrampolineMachineCode);
    }

    void DetourCreator::AddNops(std::vector<BYTE>& machineCode, const size_t neededLength) const {
        for(size_t length = machineCode.size(); length < neededLength; ++length) {
            machineCode.push_back(_nopCode);
        }
    }
}
