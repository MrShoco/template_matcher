#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "tbadstringexception.h"
#include "tnotsupportedexception.h"
#include "t2dsingletemplatematcher.h"
#include "tnaivetemplatematcher.h"
#include "tsingletemplatematcher.h"
#include "stringstream.h"
#include "filestream.h"
#include "randomcharstream.h"

TStringId T2DSingleTemplateMatcher::AddTemplate(const std::vector<std::string> &templ_) {
    if (!templ.empty())
        throw TNotSupportedException("You can't add more than one template");

    if (templ_.size() == 0)
        throw TBadStringException("Empty template");

    for (size_t i = 0; i < templ_.size(); i++) {
        for (size_t j = 0; j < templ_[i].size(); j++) {
            if((unsigned char)templ_[i][j] < 32)
                throw TBadStringException("Some chars in template are not in range 32-255");
        }
    }

    templ = templ_;
    ids.resize(templ_.size());
    for (size_t i = 0; i < templ_.size(); i++) {
        ids[i] = static_matcher.AddTemplate(templ_[i]);
    }
    kmp.resize(ids.size());
    kmp[0] = 0;
    for (size_t i = 1; i < ids.size(); i++) {
        int j = kmp[i - 1];
        while (kmp[i] > 0 && ids[i] != ids[j]) {
            j = kmp[j - 1];
        }
        if (ids[i] == ids[j]) j++;
        kmp[i] = j;
    }

    return 0;
}

T2DMatchResults T2DSingleTemplateMatcher::MatchStream(const std::vector<std::string> &match) {
    std::string current;
    T2DMatchResults result;
    size_t pref = 0, n = 0;

    if (templ.empty())
        throw TNotSupportedException("You need to add at least one template");

    std::vector<std::pair<size_t, int> > static_matched;
    table.resize(match.size(), std::vector<int>(match[0].size(), -1));

    for (size_t i = 0; i < match.size(); i++) {
        StringStream stm(match[i]);
        static_matched = static_matcher.MatchStream(stm);
        for (size_t j = 0; j < static_matched.size(); j++) {
            table[i][static_matched[j].first - 1] = static_matched[j].second;
        }
    }

    for (size_t i = 0; i < table[0].size(); i++) {
        int pref = 0;
        for (size_t j = 0; j < table.size(); j++) {
            while (pref > 0 && table[j][i] != ids[pref])
                pref = kmp[pref - 1];
            if (table[j][i] == ids[pref])
                pref++;
            
            if(pref == ids.size())
                result.push_back(std::make_pair(j + 1, i + 1));
        }
    }

    return result;
}

