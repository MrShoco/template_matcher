#include "randomcharstream.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <cstdlib>

bool RandomCharStream::IsEmpty() const {
    return ind >= sz_;
}

char RandomCharStream::GetChar() {
    if(IsEmpty())
        throw std::out_of_range("Stream ends");
    ind++;

    return rand() % (range_max - range_min + 1) + range_min;
}

void RandomCharStream::Reset() {
    srand(rand_seed);
    ind = 0;
}

class RandomCharStreamTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        stm1 = RandomCharStream(5);

        stm2 = RandomCharStream(0);

        stm3 = RandomCharStream(1e5 + 5, 120, 200);
    }

    RandomCharStream stm1;
    RandomCharStream stm2;
    RandomCharStream stm3;
};

TEST_F(RandomCharStreamTest, TestClass) {
    ASSERT_FALSE(stm1.IsEmpty());
    ASSERT_TRUE(stm2.IsEmpty());
    ASSERT_FALSE(stm3.IsEmpty());

    ASSERT_THROW(RandomCharStream(10, 255, 32), std::range_error);
    ASSERT_THROW(stm2.GetChar(), std::out_of_range);

    std::string check;
    while(!stm1.IsEmpty())
    {
        ASSERT_NO_THROW(check += stm1.GetChar());
    }
    ASSERT_EQ(check.size(), 5);

    for(int i = 0; i < 1e4; i++)
    {
        char c;
        ASSERT_NO_THROW(c = stm3.GetChar());
        ASSERT_LE((unsigned char)c, 200);
        ASSERT_GE((unsigned char)c, 120);
    }
}

