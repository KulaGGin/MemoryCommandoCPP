#include "pch.h"
#include "MemoryCommando/MemoryCommando.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}


namespace MemoryCommandoCPPTests {
    class MemoryCommandoInternalTest: public ::testing::Test {
    protected:
        void SetUp() override {
            MemoryCommando::MemoryCommando memoryCommando{};
        }

        void TearDown() override {
        
        }
    public:
        //TEST_F(MemoryCommandoInternalTest, ReadVirtualMemoryArrayOfBytes);
    };
}
