#include "gtest/gtest.h"

#include <Windows.h>

#include "Memory/Internal/MemoryManagerInternal.h"

namespace MemoryCommando::GoogleTests {
    using namespace Memory::Internal;

    class MemoryManagerInternalTests : public ::testing::Test {
    public:
        MemoryManagerInternal MemoryManagerInternal{};
    };

    TEST_F(MemoryManagerInternalTests, CanCreateMemoryManagerInternal) {
        
    }

    TEST_F(MemoryManagerInternalTests, GetsCorrectProcessID) {
        auto currentProcessID = GetCurrentProcessId();

        EXPECT_EQ(currentProcessID, MemoryManagerInternal.GetProcessId());
    }

}

