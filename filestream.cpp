#include "filestream.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <iostream>
#include <ios>

bool FileStream::IsEmpty() const {
    if (!((*fin).is_open()))
        throw std::logic_error("No such file");

    return (*fin).eof();
}

char FileStream::GetChar() {
    if (IsEmpty())
        throw std::out_of_range("Stream ends");

    return (*fin).get();
}

void FileStream::Reset() {
    (*fin).clear();
    (*fin).seekg(0, std::ios::beg);
}

class FileStreamTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        stm1 = FileStream("filestream.cpp");
        stm2 = FileStream("");
    }

    FileStream stm1;
    FileStream stm2;
};

TEST_F(FileStreamTest, TestClass) {
    ASSERT_FALSE(stm1.IsEmpty());
    ASSERT_THROW(stm2.GetChar(), std::logic_error);

    while(!stm1.IsEmpty()) {
        ASSERT_NO_THROW(stm1.GetChar());
    }
}

