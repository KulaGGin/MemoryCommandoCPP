#include "pch.h"
#include "MemoryCommando/MemoryCommando.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}


namespace MemoryCommandoCPPTests {
    class MemoryCommandoTests: public ::testing::Test {
    protected:
        void SetUp() override {

        }

        void TearDown() override {
        
        }

    };
}
