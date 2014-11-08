#include "stringstream.h"
#include <gtest/gtest.h>
#include <stdexcept>

bool StringStream::IsEmpty() const {
    return ind >= stream_.size();
}

char StringStream::GetChar() {
    if(IsEmpty())
        throw std::out_of_range("Stream ends");
    return stream_[ind++];
}

void StringStream::AddChar(char c) {
    stream_ += c;
}

class StringStreamTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        stm1 = StringStream("abacaba");

        stm2 = StringStream("");

        stm3 = StringStream("");
    }

    StringStream stm1;
    StringStream stm2;
    StringStream stm3;
};

TEST_F(StringStreamTest, TestClass) {
    ASSERT_FALSE(stm1.IsEmpty());
    ASSERT_TRUE(stm2.IsEmpty());
    ASSERT_TRUE(stm3.IsEmpty());

    ASSERT_THROW(stm2.GetChar(), std::out_of_range);

    std::string check;
    while(!stm1.IsEmpty())
    {
        ASSERT_NO_THROW(check += stm1.GetChar());
    }
    ASSERT_EQ(check, "abacaba");

    for(int i = 0; i < 256; i++)
        stm3.AddChar(i);

    for(int i = 0; i < 256; i++)
    {
        ASSERT_NO_THROW(ASSERT_EQ(char(i), stm3.GetChar()));
    }
}

