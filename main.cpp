#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <gtest/gtest.h>
#include "tnotsupportedexception.h"
#include "tsingletemplatematcher.h"
#include "tstatictemplatematcher.h"
#include "tnaivetemplatematcher.h"
#include "stringstream.h"
#include "filestream.h"
#include "randomcharstream.h"

class TStaticTemplateMatcherTest : public ::testing::Test {
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
    TStaticTemplateMatcher static_matcher;

    StringStream stm1;
    FileStream stm2;
    RandomCharStream stm3;

    std::vector<std::pair<size_t, int> > single_matched;
    std::vector<std::pair<size_t, int> > naive_matched;
    std::vector<std::pair<size_t, int> > static_matched;
};

TEST_F(TStaticTemplateMatcherTest, TestForCorrect) {
    ASSERT_THROW(static_matcher.MatchStream(stm1), TNotSupportedException);
    static_matcher.AddTemplate("aba");

    static_matched = static_matcher.MatchStream(stm3);
    for (size_t i = 0; i < static_matched.size(); i++)
    {
        ASSERT_GE(static_matched[i].second, 0);
        ASSERT_LT(static_matched[i].second, 1);
        ASSERT_GE(static_matched[i].first, 3);
        ASSERT_LE(static_matched[i].first, 100);
    }


    static_matched = static_matcher.MatchStream(stm1);
    ASSERT_EQ(static_matched.size(), 2);

    static_matched = static_matcher.MatchStream(stm2);
    ASSERT_EQ(static_matched.size(), 200);

    ASSERT_THROW(static_matcher.AddTemplate("a"), TNotSupportedException);
}

TEST_F(TStaticTemplateMatcherTest, StressTesting) {
    naive_matcher.AddTemplate("aba");
    naive_matcher.AddTemplate("aca");
    naive_matcher.AddTemplate("a");
    static_matcher.AddTemplate("aba");
    static_matcher.AddTemplate("aca");
    static_matcher.AddTemplate("a");

// String stream test
    naive_matched = naive_matcher.MatchStream(stm1);
    stm1.Reset();
    static_matched = static_matcher.MatchStream(stm1);

    
    std::sort(naive_matched.begin(), naive_matched.end());
    std::sort(static_matched.begin(), static_matched.end());

    ASSERT_EQ(static_matched, naive_matched);

// File stream test
    static_matcher = TStaticTemplateMatcher();

    single_matcher.AddTemplate("aba");
    static_matcher.AddTemplate("aba");

    single_matched = single_matcher.MatchStream(stm2);
    stm2.Reset();
    static_matched = static_matcher.MatchStream(stm2);

    std::sort(single_matched.begin(), single_matched.end());
    std::sort(static_matched.begin(), static_matched.end());

    ASSERT_EQ(static_matched, single_matched);

// Random test
    static_matcher = TStaticTemplateMatcher();

    static_matcher.AddTemplate("aba");
    static_matcher.AddTemplate("aca");
    static_matcher.AddTemplate("a");

    for (size_t i = 1; i < 1000; i++) {
        stm3 = RandomCharStream(i, 'a', 'c');
        naive_matched = naive_matcher.MatchStream(stm3);
        stm3.Reset();
        static_matched = static_matcher.MatchStream(stm3);
        
        std::sort(naive_matched.begin(), naive_matched.end());
        std::sort(static_matched.begin(), static_matched.end());

        ASSERT_EQ(static_matched, naive_matched);
    }
}


TEST_F(TStaticTemplateMatcherTest, BigTest) {
    for (size_t i = 1; i < 1000; i++) {
        size_t len = rand() % i + 1;
        std::string s;
        for(size_t j = 0; j < len; j++)
            s += rand()%('z' - 'a' + 1) + 'a';

        static_matcher.AddTemplate(s);
    }

    stm3 = RandomCharStream(100000, 'a', 'z');

    static_matched = static_matcher.MatchStream(stm3);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
