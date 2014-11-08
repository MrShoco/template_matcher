#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <gtest/gtest.h>
#include "tnotsupportedexception.h"
#include "tsingletemplatematcher.h"
#include "tnaivetemplatematcher.h"
#include "stringstream.h"
#include "filestream.h"
#include "randomcharstream.h"

class TSingleTemplateMatcherTest : public ::testing::Test {
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

    TSingleTemplateMatcher single_matcher;
    TNaiveTemplateMatcher naive_matcher;

    StringStream stm1;
    FileStream stm2;
    RandomCharStream stm3;

    std::vector<std::pair<size_t, int> > single_matched;
    std::vector<std::pair<size_t, int> > naive_matched;
};

TEST_F(TSingleTemplateMatcherTest, TestForCorrect) {

    single_matcher.AddTemplate("aba");
    ASSERT_THROW(single_matcher.AddTemplate("a"), TNotSupportedException);

    single_matched = single_matcher.MatchStream(stm3);
    for (size_t i = 0; i < single_matched.size(); i++)
    {
        ASSERT_GE(single_matched[i].second, 0);
        ASSERT_LT(single_matched[i].second, 1);
        ASSERT_GE(single_matched[i].first, 3);
        ASSERT_LE(single_matched[i].first, 100);
    }


    single_matched = single_matcher.MatchStream(stm1);
    ASSERT_EQ(single_matched.size(), 2);

    single_matched = single_matcher.MatchStream(stm2);
    ASSERT_EQ(single_matched.size(), 200);
}

TEST_F(TSingleTemplateMatcherTest, StressTesting) {
    single_matcher.AddTemplate("aba");
    naive_matcher.AddTemplate("aba");

// String stream test
    single_matched = single_matcher.MatchStream(stm1);
    stm1.Reset();
    naive_matched = naive_matcher.MatchStream(stm1);
    
    std::sort(single_matched.begin(), single_matched.end());
    std::sort(naive_matched.begin(), naive_matched.end());

    ASSERT_EQ(naive_matched, single_matched);

// File stream test
    single_matched = single_matcher.MatchStream(stm2);
    stm2.Reset();
    naive_matched = naive_matcher.MatchStream(stm2);

    std::sort(single_matched.begin(), single_matched.end());
    std::sort(naive_matched.begin(), naive_matched.end());

    ASSERT_EQ(naive_matched, single_matched);

// Random test
    for (size_t i = 1; i < 1000; i++) {
        stm3 = RandomCharStream(i, 'a', 'c');
        single_matched = single_matcher.MatchStream(stm3);
        stm3.Reset();
        naive_matched = naive_matcher.MatchStream(stm3);
        
        std::sort(single_matched.begin(), single_matched.end());
        std::sort(naive_matched.begin(), naive_matched.end());

        ASSERT_EQ(naive_matched, single_matched);
    }

}



int main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
