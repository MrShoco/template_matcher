#ifndef I_META_TEMPLATE_MATCHER
#define I_META_TEMPLATE_MATCHER

#include <vector>
#include <iostream>
#include "icharstream.h"

typedef int TStringId;
typedef std::pair<std::size_t, TStringId> TOccurence;
typedef std::vector<TOccurence> TMatchResults;

class IMetaTemplateMatcher {
public:
    virtual TStringId AddTemplate(const std::string &temp) = 0;
    virtual TMatchResults MatchStream(ICharStream &stream) = 0;
    virtual ~IMetaTemplateMatcher() = default;
};

#endif
