#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "tbadstringexception.h"
#include "tnotsupportedexception.h"
#include "tsingletemplatematcher.h"
#include "tnaivetemplatematcher.h"
#include "stringstream.h"
#include "filestream.h"
#include "randomcharstream.h"

TStringId TSingleTemplateMatcher::AddTemplate(const std::string &templ) {
    if (!templ_.empty())
        throw TNotSupportedException("You can't add more than one template");

    if (templ.size() == 0)
        throw TBadStringException("Empty template");

    for (size_t i = 0; i < templ.size(); i++) {
        if((unsigned char)templ[i] < 32)
            throw TBadStringException("Some chars in template are not in range 32-255");
    }

    templ_ = templ;

    kmp.resize(templ_.size());
    kmp[0] = 0;

    for (size_t i = 1; i < templ_.size(); i++) {
        size_t j = kmp[i];
        while (j > 0 && templ_[i] != templ_[j])
            j = kmp[j - 1];
        if (templ_[i] == templ_[j])
            j++;
        kmp[i] = j;
    }

    return 0;
}

TMatchResults TSingleTemplateMatcher::MatchStream(ICharStream &stream) {
    std::string current;
    TMatchResults result;
    size_t pref = 0, n = 0;

    if (templ_.empty())
        throw TNotSupportedException("You need to add at least one template");

    while (!stream.IsEmpty()) {
        n++;
        char c = stream.GetChar();
        current += c;

        if ((unsigned char)c < 32)
        {
            throw TBadStringException("Some chars in stream are not in range 32-255");
        }
        
        while (pref > 0 && c != templ_[pref])
            pref = kmp[pref - 1];
        if (c == templ_[pref])
            pref++;

        if (pref == templ_.size())
            result.push_back(std::make_pair(n, 0));
    }

    return result;
}

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



