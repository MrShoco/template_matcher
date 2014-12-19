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

class TSingleTemplateMatcherAppendTest : public ::testing::Test {
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
    TSingleTemplateMatcher single_append_matcher;
    TSingleTemplateMatcher asingle_matcher;

    StringStream stm1;
    FileStream stm2;
    RandomCharStream stm3;

    std::vector<std::pair<size_t, int> > single_matched;
    std::vector<std::pair<size_t, int> > single_append_matched;
    std::vector<std::pair<size_t, int> > asingle_matched;
};

TEST_F(TSingleTemplateMatcherAppendTest, AppendCharToTemplateTest) {

    single_matcher.AddTemplate("aba");
    asingle_matcher.AddTemplate("a");
    asingle_matcher.AppendCharToTemplate('b');
    asingle_matcher.AppendCharToTemplate('a');

    single_matched = single_matcher.MatchStream(stm1);
    stm1.Reset();
    asingle_matched = asingle_matcher.MatchStream(stm1);
    
    std::sort(single_matched.begin(), single_matched.end());
    std::sort(asingle_matched.begin(), asingle_matched.end());

    ASSERT_EQ(asingle_matched, single_matched);

// Random Test

    asingle_matcher.AppendCharToTemplate('c');
    single_matcher = TSingleTemplateMatcher();
    single_matcher.AddTemplate("abac");

    for (size_t i = 1; i < 1000; i++) {
        stm3 = RandomCharStream(i, 'a', 'c');
        single_matched = single_matcher.MatchStream(stm3);
        stm3.Reset();
        asingle_matched = asingle_matcher.MatchStream(stm3);
        
        std::sort(single_matched.begin(), single_matched.end());
        std::sort(asingle_matched.begin(), asingle_matched.end());

        ASSERT_EQ(asingle_matched, single_matched);
    }
}

TEST_F(TSingleTemplateMatcherAppendTest, BigAppendTest) {
    std::string s;
    for(size_t j = 0; j < 10000; j++)
        s += rand()%('z' - 'a' + 1) + 'a';

    single_append_matcher.AddTemplate(s);
    single_matcher.AddTemplate(s);

    for(size_t i = 0; i < 100; i++) {
        char c = rand()%('z' - 'a' + 1) + 'a';
        s += c;
        single_append_matcher.AppendCharToTemplate(c);
        single_matcher = TSingleTemplateMatcher();
        single_matcher.AddTemplate(s);

        stm3 = RandomCharStream(100000, 'a', 'z');

        single_append_matched = single_append_matcher.MatchStream(stm3);
        single_matched = single_matcher.MatchStream(stm3);
    }
}



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
