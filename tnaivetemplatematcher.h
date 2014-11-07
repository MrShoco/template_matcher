#ifndef T_NATIVE_TEMPLATE_MATCHER
#define T_NATIVE_TEMPLATE_MATCHER

#include <vector>
#include <iostream>
#include "imetatemplatematcher.h"

class TNaiveTemplateMatcher : public IMetaTemplateMatcher {
public:
    TStringId AddTemplate(const std::string &temp);
    TMatchResults MatchStream(ICharStream &stream);
private:
    std::vector<std::string> temps;
};

#endif
