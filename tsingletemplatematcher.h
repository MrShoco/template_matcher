#ifndef T_SINGLE_TEMPLATE_MATCHER
#define T_SINGLE_TEMPLATE_MATCHER

#include <vector>
#include <iostream>
#include "imetatemplatematcher.h"

class TSingleTemplateMatcher : public IMetaTemplateMatcher {
public:
    TStringId AddTemplate(const std::string &temp);
    TMatchResults MatchStream(ICharStream &stream);
    void AppendCharToTemplate(char c);
    void PrependCharToTemplate(char c);
private:
    std::string templ_ = "";
    std::string prepended = "";
    std::vector<int> kmp;
};

#endif
