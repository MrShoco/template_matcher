#include <gtest/gtest.h>
#include <queue>
#include "tnaivetemplatematcher.h"
#include "tbadstringexception.h"
#include "stringstream.h"
#include "filestream.h"
#include "randomcharstream.h"

TStringId TNaiveTemplateMatcher::AddTemplate(const std::string &templ) {
    if (templ.size() == 0)
        throw TBadStringException("Empty template");

    for(size_t i = 0; i < templ.size(); i++) {
        if((unsigned char)templ[i] < 32)
            throw TBadStringException("Some chars in template are not in range 32-255");
    }

    templs.push_back(templ);
    max_size = std::max(max_size, templ.size());
    return (int)templs.size() - 1;
}

TMatchResults TNaiveTemplateMatcher::MatchStream(ICharStream &stream) {
    std::string current;
    TMatchResults result;
    size_t n = 0;

    while (!stream.IsEmpty()) {
        n++;
        char c = stream.GetChar();
        current += c;

        if(current.size() > max_size)
            current = current.substr(1, current.size() - 1);

        if ((unsigned char)c < 32)
        {
            throw TBadStringException("Some chars in stream are not in range 32-255");
        }

        for (size_t i = 0; i < templs.size(); i++) {
            if (current.size() >= templs[i].size()) {
                bool eq = true;
                for (size_t j = 0; j < templs[i].size(); j++) {
                    if (templs[i][j] != current[current.size() - templs[i].size() + j])
                        eq = false;
                }
                if(eq == true) {
                    result.push_back(std::make_pair(n, i));
                }
            }
        }
    }

    return result;
}

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


