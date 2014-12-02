#include "tstatictemplatematcher.h"
#include "tbadstringexception.h"
#include "tnotsupportedexception.h"

int TStaticTemplateMatcher::ida = 0;

TStringId TStaticTemplateMatcher::AddTemplate(const std::string &templ) {
    if (n == 0)
        root = new Vertex(root, 0);

    if (matched)
        throw TNotSupportedException("Adding templates after match not supported");

    if (templ.size() == 0)
        throw TBadStringException("Empty template");

    for (size_t i = 0; i < templ.size(); i++) {
        if((unsigned char)templ[i] < 32)
            throw TBadStringException("Some chars in template are not in range 32-255");
    }

    return add(templ);
}

TMatchResults TStaticTemplateMatcher::MatchStream(ICharStream &stream) {
    std::string current;
    TMatchResults result;
    size_t pref = 0, n = 0;

    if (this -> n == 0)
        throw TNotSupportedException("You need to add at least one template");
    
    matched = true;

    Vertex* cur = root;

    while (!stream.IsEmpty()) {
        n++;
        char c = stream.GetChar();
        current += c;

        if ((unsigned char)c < 32)
        {
            throw TBadStringException("Some chars in stream are not in range 32-255");
        }

        cur = go(cur, c);

        Vertex* ans = cur;
        
        while(ans != root) {
            if(ans -> leaf != -1)
                result.push_back(std::make_pair(n, ans -> leaf));
            ans = get_closest(ans);
        }
    }

    return result;
}
