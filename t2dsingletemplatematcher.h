#ifndef T_2D_SINGLE_TEMPLATE_MATCHER
#define T_2D_SINGLE_TEMPLATE_MATCHER

#include <vector>
#include <iostream>
#include "tstatictemplatematcher.h"

typedef std::pair<std::size_t, std::size_t> T2DOccurence;
typedef std::vector<T2DOccurence> T2DMatchResults;

class T2DSingleTemplateMatcher {
public:
    TStringId AddTemplate(const std::vector<std::string> &temp);
    T2DMatchResults MatchStream(const std::vector<std::string> &match);
private:
    std::vector<std::string> templ;
    std::vector<std::vector<int> > table;
    std::vector<TStringId> ids; 
    std::vector<int> kmp;
    TStaticTemplateMatcher static_matcher;
};

#endif
