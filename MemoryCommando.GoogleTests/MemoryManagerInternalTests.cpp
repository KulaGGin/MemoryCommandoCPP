#include "gtest/gtest.h"

#include <Windows.h>

#include "Exceptions/BadReadPointerException.h"
#include "Exceptions/VirtualAllocException.h"
#include "Exceptions/VirtualFreeException.h"
#include "Exceptions/VirtualProtectException.h"
#include "Exceptions/VirtualQueryException.h"
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
            uintptr_t allocatedAddress = MemoryManagerInternal.AllocateVirtualMemory(wantedAllocationAddress, wantedAllocationSize, wantedProtectionType, wantedAllocationType);
            FAIL(); // Test will fail if the line above executes successfully and doesn't throw an exception.
        }
        catch(Exceptions::VirtualAllocException virtualAllocException) {
            EXPECT_EQ(virtualAllocException.BaseAddress, wantedAllocationAddress);
            EXPECT_EQ(virtualAllocException.AllocationSize, wantedAllocationSize);
            EXPECT_EQ(virtualAllocException.AllocationType, wantedAllocationType);
            EXPECT_EQ(virtualAllocException.ProtectionType, wantedProtectionType);
        }
    }

    TEST_F(MemoryManagerInternalTests, FreeMemory_ThrowsWhenFreeTypeIsMemReleaseAndSizeIsNotZero) {
        auto allocationAddress = MemoryManagerInternal.AllocateVirtualMemory(0, 0x10000);

        try {
            MemoryManagerInternal.FreeVirtualMemory(allocationAddress, MEM_RELEASE, 1);
            FAIL();
        }
        catch(std::invalid_argument invalidArgument) {
            SUCCEED();
        }
    }

    TEST_F(MemoryManagerInternalTests, FreeMemory_ThrowsWhenFreeAddressIsNotTheSameAsAllocationAddress) {
        auto allocationAddress = MemoryManagerInternal.AllocateVirtualMemory(0, 0x10000);
        auto wantedFreeAddress = allocationAddress - 0x2000;
        auto wantedMemFreeType = MEM_RELEASE;
        auto wantedMemFreeSize = 0;
        try {
            MemoryManagerInternal.FreeVirtualMemory(wantedFreeAddress, wantedMemFreeType, wantedMemFreeSize);
            FAIL();
        }
        catch(Exceptions::VirtualFreeException virtualFreeException) {
            EXPECT_EQ(virtualFreeException.Address, wantedFreeAddress);
            EXPECT_EQ(virtualFreeException.FreeType, wantedMemFreeType);
            EXPECT_EQ(virtualFreeException.Size, wantedMemFreeSize);
        }
    }

    TEST_F(MemoryManagerInternalTests, FreeMemory_FreesWhenGivenProperArguments) {
        size_t allocationSize = 0x2000;
        uintptr_t allocationAddress = MemoryManagerInternal.AllocateVirtualMemory(0, allocationSize);
        EXPECT_FALSE(IsBadReadPtr(reinterpret_cast<void*>(allocationAddress), allocationSize));

        auto wantedFreeAddress = allocationAddress;
        auto wantedMemFreeType = MEM_RELEASE;
        auto wantedMemFreeSize = 0;

        MemoryManagerInternal.FreeVirtualMemory(wantedFreeAddress, wantedMemFreeType, wantedMemFreeSize);

        for(size_t memoryByteIndex = 0; memoryByteIndex <= allocationSize; ++memoryByteIndex) {
            EXPECT_TRUE(IsBadReadPtr(reinterpret_cast<void*>(allocationAddress + memoryByteIndex), 0x1));
        }
    }

    TEST_F(MemoryManagerInternalTests, ProtectMemory_ThrowsWhenGivenTooBigAddress) {
        uintptr_t wantedProtectionAddress = -1ull;
        size_t wantedSize = 0x2000;
        auto protectionType = PAGE_EXECUTE;
        try {
            MemoryManagerInternal.ProtectVirtualMemory(wantedProtectionAddress, wantedSize, protectionType);
            FAIL();
        }
        catch(Exceptions::VirtualProtectException virtualProtectException) {
            EXPECT_EQ(virtualProtectException.Address, wantedProtectionAddress);
            EXPECT_EQ(virtualProtectException.ProtectionSize, wantedSize);
            EXPECT_EQ(virtualProtectException.ProtectionType, protectionType);
        }
    }

    TEST_F(MemoryManagerInternalTests, ProtectMemory_ProtectsWhenGivenProperArguments) {
        size_t allocationSize = 0x2000;
        uintptr_t allocationAddress = MemoryManagerInternal.AllocateVirtualMemory(0, allocationSize);
        EXPECT_FALSE(IsBadReadPtr(reinterpret_cast<void*>(allocationAddress), allocationSize));

        uintptr_t wantedProtectionAddress = allocationAddress;
        size_t wantedSize = 0x2000;
        int protectionType = PAGE_EXECUTE;

        MemoryManagerInternal.ProtectVirtualMemory(wantedProtectionAddress, wantedSize, protectionType);

        MEMORY_BASIC_INFORMATION memoryBasicInformation{};
        size_t memoryBasicInformationSize = sizeof memoryBasicInformation;
        auto numberOfBytesWritten = VirtualQuery(reinterpret_cast<LPCVOID>(allocationAddress), &memoryBasicInformation, memoryBasicInformationSize);

        EXPECT_EQ(numberOfBytesWritten, memoryBasicInformationSize) << "VirtualQuery didn't write number of bytes equal to the size of MEMORY_BASIC_INFORMATION struct";

        EXPECT_EQ(memoryBasicInformation.Protect, PAGE_EXECUTE);
    }

    TEST_F(MemoryManagerInternalTests, QueryVirtualMemory_ThrowsWhenTryingToQueryTooBigAddress) {
        uintptr_t wantedQueryAddress = -1ull;

        try {
            [[maybe_unused]] MEMORY_BASIC_INFORMATION memoryBasicInformation =
                MemoryManagerInternal.QueryVirtualMemory(wantedQueryAddress);
            FAIL();
        }
        catch(Exceptions::VirtualQueryException virtualQueryException) {
            EXPECT_EQ(virtualQueryException.Address, wantedQueryAddress);
        }
    }

    TEST_F(MemoryManagerInternalTests, QueryVirtualMemory_QueriesWhenGivenNormalAddress) {
        auto wantedAllocationSize = 0x10000;
        auto wantedAllocationType = MEM_COMMIT;
        auto wantedProtection = PAGE_READONLY;

        uintptr_t allocationAddress = MemoryManagerInternal.AllocateVirtualMemory(0, wantedAllocationSize, wantedProtection, wantedAllocationType);

        uintptr_t wantedQueryAddress = allocationAddress;

        MEMORY_BASIC_INFORMATION memoryBasicInformation = MemoryManagerInternal.QueryVirtualMemory(wantedQueryAddress);

        EXPECT_EQ(memoryBasicInformation.BaseAddress, reinterpret_cast<PVOID>(allocationAddress));
        EXPECT_EQ(memoryBasicInformation.RegionSize, wantedAllocationSize);
        EXPECT_EQ(memoryBasicInformation.AllocationProtect, wantedProtection);
        EXPECT_EQ(memoryBasicInformation.Protect, wantedProtection);
        EXPECT_TRUE(memoryBasicInformation.State & MEM_COMMIT);
    }

    TEST_F(MemoryManagerInternalTests, ReadVirtualMemory_ReadsMemoryAppropriatelyIfGivenNormalAddress) {
        int integer = 12345;
        byte* integerBytes = reinterpret_cast<byte*>(&integer);
        auto readBytes = MemoryManagerInternal.ReadVirtualMemory(reinterpret_cast<uintptr_t>(&integer), sizeof integer );

        EXPECT_EQ(readBytes.size(), sizeof integer);

        EXPECT_EQ(readBytes[0], integerBytes[0]);
        EXPECT_EQ(readBytes[1], integerBytes[1]);
        EXPECT_EQ(readBytes[2], integerBytes[2]);
        EXPECT_EQ(readBytes[3], integerBytes[3]);
    }

    TEST_F(MemoryManagerInternalTests, ReadVirtualMemory_ThrowsIfGivenBadAddress) {
        uintptr_t allocationAddress = MemoryManagerInternal.AllocateVirtualMemory(0, 0x1000, PAGE_NOACCESS);

        try {
            std::vector<BYTE> readBytes = MemoryManagerInternal.ReadVirtualMemory(allocationAddress, 0x4);
        }
        catch(Exceptions::BadReadPointerException badReadPointerException) {
            EXPECT_EQ(badReadPointerException.Address, allocationAddress);
        }
        try {
            std::vector<BYTE> readBytes = MemoryManagerInternal.ReadVirtualMemory(-1, 0x4);
        }
        catch(Exceptions::BadReadPointerException badReadPointerException) {
            EXPECT_EQ(badReadPointerException.Address, -1);
        }
    }
        }
    }
}

