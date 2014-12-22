#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <gtest/gtest.h>
#include "tnotsupportedexception.h"
#include "tsingletemplatematcher.h"
#include "tstatictemplatematcher.h"
#include "tdynamictemplatematcher.h"
#include "tnaivetemplatematcher.h"
#include "stringstream.h"
#include "filestream.h"
#include "randomcharstream.h"
#include "tbadstringexception.h"

class TDynamicTemplateMatcherTest : public ::testing::Test {
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
    TDynamicTemplateMatcher dynamic_matcher;

    StringStream stm1;
    FileStream stm2;
    RandomCharStream stm3;

    std::vector<std::pair<size_t, int> > single_matched;
    std::vector<std::pair<size_t, int> > naive_matched;
    std::vector<std::pair<size_t, int> > static_matched;
    std::vector<std::pair<size_t, int> > dynamic_matched;
};

TEST_F(TDynamicTemplateMatcherTest, TestForCorrect) {
    ASSERT_THROW(dynamic_matcher.MatchStream(stm1), TNotSupportedException);
    dynamic_matcher.AddTemplate("aba");

    dynamic_matched = dynamic_matcher.MatchStream(stm3);
    for (size_t i = 0; i < dynamic_matched.size(); i++)
    {
        ASSERT_GE(dynamic_matched[i].second, 0);
        ASSERT_LT(dynamic_matched[i].second, 1);
        ASSERT_GE(dynamic_matched[i].first, 3);
        ASSERT_LE(dynamic_matched[i].first, 100);
    }


    dynamic_matched = dynamic_matcher.MatchStream(stm1);
    ASSERT_EQ(dynamic_matched.size(), 2);

    dynamic_matched = dynamic_matcher.MatchStream(stm2);
    ASSERT_EQ(dynamic_matched.size(), 200);

    ASSERT_NO_THROW(dynamic_matcher.AddTemplate("a"));
    ASSERT_NO_THROW(dynamic_matcher.MatchStream(stm3));
}

TEST_F(TDynamicTemplateMatcherTest, StressTesting) {
    std::vector<std::string> templs({"aba", "aca", "a"});

// String stream test
    for (size_t i = 0; i < templs.size(); i++) {
        naive_matcher.AddTemplate(templs[i]);
        dynamic_matcher.AddTemplate(templs[i]);
        naive_matched = naive_matcher.MatchStream(stm1);
        stm1.Reset();
        dynamic_matched = dynamic_matcher.MatchStream(stm1);
        stm1.Reset();

        
        std::sort(naive_matched.begin(), naive_matched.end());
        std::sort(dynamic_matched.begin(), dynamic_matched.end());

        ASSERT_EQ(dynamic_matched, naive_matched);
    }

// File stream test
    dynamic_matcher = TDynamicTemplateMatcher();

    single_matcher.AddTemplate("aba");
    dynamic_matcher.AddTemplate("aba");

    single_matched = single_matcher.MatchStream(stm2);
    stm2.Reset();
    dynamic_matched = dynamic_matcher.MatchStream(stm2);

    std::sort(single_matched.begin(), single_matched.end());
    std::sort(dynamic_matched.begin(), dynamic_matched.end());

    ASSERT_EQ(dynamic_matched, single_matched);

// Random test
    dynamic_matcher = TDynamicTemplateMatcher();
    naive_matcher = TNaiveTemplateMatcher();

    for (size_t i = 1; i < 100; i++) {
        std::string s;
        RandomCharStream stm(rand() % ((int)sqrt(i)) + 1, 'a', 'd');
        while (!stm.IsEmpty())
            s += stm.GetChar();
        try {
            dynamic_matcher.AddTemplate(s);
            naive_matcher.AddTemplate(s);
        } catch(TBadStringException &e) {
            continue;
        }

        stm3 = RandomCharStream(i, 'a', 'd');
        naive_matched = naive_matcher.MatchStream(stm3);
        stm3.Reset();
        dynamic_matched = dynamic_matcher.MatchStream(stm3);
        
        std::sort(naive_matched.begin(), naive_matched.end());
        std::sort(dynamic_matched.begin(), dynamic_matched.end());

        ASSERT_EQ(dynamic_matched, naive_matched);
    }
}


TEST_F(TDynamicTemplateMatcherTest, BigTest) {
    for (size_t i = 1; i < 500; i++) {
        size_t len = rand() % i + 1;
        std::string s;
        for(size_t j = 0; j < len; j++)
            s += rand()%('z' - 'a' + 1) + 'a';

        try {
            dynamic_matcher.AddTemplate(s);
            static_matcher.AddTemplate(s);
        } catch(TBadStringException &e) {
            continue;
        }
    }

    stm3 = RandomCharStream(100000, 'a', 'z');

    dynamic_matched = dynamic_matcher.MatchStream(stm3);
    stm3.Reset();
    static_matched = static_matcher.MatchStream(stm3);

    std::sort(static_matched.begin(), static_matched.end());
    std::sort(dynamic_matched.begin(), dynamic_matched.end());

    ASSERT_EQ(dynamic_matched, static_matched);
    

    dynamic_matcher = TDynamicTemplateMatcher();
    
    std::vector<std::string> strs;

    for (size_t i = 1; i < 100; i++) {
        size_t len = rand() % i + 1;
        std::string s;
        for(size_t j = 0; j < len; j++)
            s += rand()%('z' - 'a' + 1) + 'a';
        try {
            dynamic_matcher.AddTemplate(s);
            strs.push_back(s);
        } catch(TBadStringException &e) {
            continue;
        }

        static_matcher = TStaticTemplateMatcher();
        
        for (size_t j = 0; j < strs.size(); j++) {
            static_matcher.AddTemplate(strs[j]);
        }
        stm3 = RandomCharStream(1000, 'a', 'z');

        dynamic_matched = dynamic_matcher.MatchStream(stm3);
        stm3.Reset();
        static_matched = static_matcher.MatchStream(stm3);

        std::sort(static_matched.begin(), static_matched.end());
        std::sort(dynamic_matched.begin(), dynamic_matched.end());

        ASSERT_EQ(dynamic_matched, static_matched);
    }

}



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
