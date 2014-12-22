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
#include "t2dsingletemplatematcher.h"
#include "stringstream.h"
#include "filestream.h"
#include "randomcharstream.h"

class T2DSingleTemplateMatcherTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }

    T2DSingleTemplateMatcher t2d_matcher;

    std::vector<std::pair<size_t, size_t> > t2d_matched;
};

TEST_F(T2DSingleTemplateMatcherTest, TestForCorrect) {
    std::vector<std::string> a;
    a.push_back("11");
    a.push_back("12");
    std::vector<std::string> b;
    b.push_back("11111");
    b.push_back("12112");
    b.push_back("11122");
    b.push_back("12111");

    t2d_matcher.AddTemplate(a);
    ASSERT_THROW(t2d_matcher.AddTemplate(a), TNotSupportedException);

    t2d_matched = t2d_matcher.MatchStream(b);

    std::sort(t2d_matched.begin(), t2d_matched.end());

    std::vector<std::pair<size_t, size_t> > correct;
    correct.push_back(std::make_pair(2, 2));
    correct.push_back(std::make_pair(2, 5));
    correct.push_back(std::make_pair(3, 4));
    correct.push_back(std::make_pair(4, 2));

    ASSERT_EQ(t2d_matched, correct);
}

TEST_F(T2DSingleTemplateMatcherTest, BigTest) {
    int n = 1000;
    std::string s;
    for (int i = 0; i < n/2; i++) {
        s += "1";
    }
    std::vector<std::string> a;
    for (int i = 0; i < n/2; i++) {
        a.push_back(s);
    }

    s = "";
    for (int i = 0; i < n; i++) {
        s += "1";
    }
    std::vector<std::string> b;
    for (int i = 0; i < n; i++) {
        b.push_back(s);
    }

    t2d_matcher.AddTemplate(a);

    t2d_matched = t2d_matcher.MatchStream(b);

    std::sort(t2d_matched.begin(), t2d_matched.end());
    
    for (int i = 0; i < t2d_matched.size(); i++) {
        ASSERT_EQ(t2d_matched[i].first, 500 + i / 501);
        ASSERT_EQ(t2d_matched[i].second, 500 + i % 501);
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
