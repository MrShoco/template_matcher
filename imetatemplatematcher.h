#ifndef I_META_TEMPLATE_MATCHER
#define I_META_TEMPLATE_MATCHER

#include <vector>

typedef int TStringId;
typedef std::pair<size_t, TStringId> TOccurence;
typedef std::vector<TOccurence> TMatchResults;

class IMetaTemplateMatcher {
public:
    virtual TStringId AddTemplate(const std::string &temp);
    virtual TMatchResults MatchStream(ICharStream &stream);
    virtual ~IMetaTemplateMatcher();
};

#endif
