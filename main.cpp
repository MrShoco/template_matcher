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

class TSingleTemplateMatcherPrependTest : public ::testing::Test {
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
    TSingleTemplateMatcher single_prepend_matcher;
    TSingleTemplateMatcher psingle_matcher;

    StringStream stm1;
    FileStream stm2;
    RandomCharStream stm3;

    std::vector<std::pair<size_t, int> > single_matched;
    std::vector<std::pair<size_t, int> > single_prepend_matched;
    std::vector<std::pair<size_t, int> > psingle_matched;
};

TEST_F(TSingleTemplateMatcherPrependTest, PrependCharToTemplateTest) {

    single_matcher.AddTemplate("aba");
    psingle_matcher.AddTemplate("a");
    psingle_matcher.PrependCharToTemplate('b');
    psingle_matcher.PrependCharToTemplate('a');

    single_matched = single_matcher.MatchStream(stm1);
    stm1.Reset();
    psingle_matched = psingle_matcher.MatchStream(stm1);
    
    std::sort(single_matched.begin(), single_matched.end());
    std::sort(psingle_matched.begin(), psingle_matched.end());

    ASSERT_EQ(psingle_matched, single_matched);

// Random Test

    psingle_matcher.PrependCharToTemplate('c');
    single_matcher = TSingleTemplateMatcher();
    single_matcher.AddTemplate("caba");

    for (size_t i = 1; i < 1000; i++) {
        stm3 = RandomCharStream(i, 'a', 'c');
        single_matched = single_matcher.MatchStream(stm3);
        stm3.Reset();
        psingle_matched = psingle_matcher.MatchStream(stm3);
        
        std::sort(single_matched.begin(), single_matched.end());
        std::sort(psingle_matched.begin(), psingle_matched.end());

        ASSERT_EQ(psingle_matched, single_matched);
    }
}

TEST_F(TSingleTemplateMatcherPrependTest, BigPrependTest) {
    std::string s;
    for(size_t j = 0; j < 10000; j++)
        s += rand()%('z' - 'a' + 1) + 'a';

    single_prepend_matcher.AddTemplate(s);
    single_matcher.AddTemplate(s);

    for(size_t i = 0; i < 100; i++) {
        char c = rand()%('z' - 'a' + 1) + 'a';
        s += c;
        single_prepend_matcher.AppendCharToTemplate(c);
        c = rand()%('z' - 'a' + 1) + 'a';
        s = c + s;
        single_prepend_matcher.PrependCharToTemplate(c);
        single_matcher = TSingleTemplateMatcher();
        single_matcher.AddTemplate(s);

        stm3 = RandomCharStream(100000, 'a', 'z');

        single_prepend_matched = single_prepend_matcher.MatchStream(stm3);
        single_matched = single_matcher.MatchStream(stm3);
        ASSERT_EQ(single_prepend_matched, single_matched);
    }
}



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
