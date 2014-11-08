#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <gtest/gtest.h>
#include "tnaivetemplatematcher.h"
#include "stringstream.h"
#include "filestream.h"
#include "randomcharstream.h"

class TNaiveTemplateMatcherTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        stm1 = StringStream("abacaba");

        std::ofstream fout("input.txt");
        for (size_t i = 0; i < 100; i++)
            fout << "abacaba";
        fout.close();

        stm2 = FileStream("input.txt");

        stm3 = RandomCharStream(100, 'a', 'c');
    }

    TNaiveTemplateMatcher matcher;

    StringStream stm1;
    FileStream stm2;
    RandomCharStream stm3;

    std::vector<std::pair<size_t, int> > matched;
};

TEST_F(TNaiveTemplateMatcherTest, TestForCorrect) {
    std::vector<std::string> strs;
    strs.push_back("a");
    strs.push_back("aba");
    strs.push_back("aca");
    strs.push_back("abacaba");

    for(size_t i = 0; i < strs.size(); i++)
        matcher.AddTemplate(strs[i]);

    matched = matcher.MatchStream(stm3);
    for (size_t i = 0; i < matched.size(); i++)
    {
        ASSERT_GE(matched[i].second, 0);
        ASSERT_LT(matched[i].second, strs.size());
        ASSERT_GE(matched[i].first, strs[matched[i].second].size());
        ASSERT_LE(matched[i].first, 100);
    }


    matched = matcher.MatchStream(stm1);
    ASSERT_EQ(matched.size(), 8);

    matched = matcher.MatchStream(stm2);
    ASSERT_EQ(matched.size(), 800);
}

int main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
