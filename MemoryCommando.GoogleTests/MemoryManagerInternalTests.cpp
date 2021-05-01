#include "gtest/gtest.h"

#include <Windows.h>

#include "Exceptions/VirtualAllocException.h"
#include "Memory/Internal/MemoryManagerInternal.h"

namespace MemoryCommando::GoogleTests {
    using namespace Memory::Internal;

    class MemoryManagerInternalTests : public ::testing::Test {
    public:
        MemoryManagerInternal MemoryManagerInternal{};
    };

    TEST_F(MemoryManagerInternalTests, GetsCorrectProcessID) {
        auto currentProcessID = GetCurrentProcessId();

        EXPECT_EQ(currentProcessID, MemoryManagerInternal.GetProcessId());
    }

    TEST_F(MemoryManagerInternalTests, GetCorrectProcessHandle) {
        HANDLE currentProcessHandle = GetCurrentProcess();
        HANDLE memoryManagerProcessHandle = MemoryManagerInternal.GetProcessHandle();

        EXPECT_EQ(currentProcessHandle, memoryManagerProcessHandle);
    }

    TEST_F(MemoryManagerInternalTests, AllocateMemory_SuccessfulWhenGivenAdequateAddress) {
        uintptr_t allocatedAddress = MemoryManagerInternal.AllocateVirtualMemory(0, 0x10000);
        EXPECT_NE(allocatedAddress, 0);
    }

    TEST_F(MemoryManagerInternalTests, AllocateMemory_ThrowsWhenGivenTooBigAddress) {
        uintptr_t wantedAllocationAddress = -1ull;
        int wantedAllocationSize = 0x10000;
        int wantedAllocationType = MEM_RESERVE | MEM_COMMIT;
        int wantedProtectionType = PAGE_READONLY;

        try {
            uintptr_t allocatedAddress = MemoryManagerInternal.AllocateVirtualMemory(wantedAllocationAddress, wantedAllocationSize, wantedAllocationType, wantedProtectionType);
            FAIL(); // Test will fail if the line above executes successfully and doesn't throw an exception.
        }
        catch(Exceptions::VirtualAllocException virtualAllocException) {
            EXPECT_EQ(virtualAllocException.BaseAddress, wantedAllocationAddress);
            EXPECT_EQ(virtualAllocException.AllocationSize, wantedAllocationSize);
            EXPECT_EQ(virtualAllocException.AllocationType, wantedAllocationType);
            EXPECT_EQ(virtualAllocException.ProtectionType, wantedProtectionType);
        }
    }

}

