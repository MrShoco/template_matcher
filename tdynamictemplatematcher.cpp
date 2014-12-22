#include "tdynamictemplatematcher.h"
#include "tbadstringexception.h"
#include "tnotsupportedexception.h"

int TDynamicTemplateMatcher::ida = 0;

TStringId TDynamicTemplateMatcher::AddTemplate(const std::string &templ) {
    if (templ.size() == 0)
        throw TBadStringException("Empty template");

    for (size_t i = 0; i < templ.size(); i++) {
        if((unsigned char)templ[i] < 32)
            throw TBadStringException("Some chars in template are not in range 32-255");
    }
    Tree cur = Create(templ, n);
    
    for (size_t i = 0; i < trees.size(); i++) {
        for (size_t j = 0; j < trees[i].strings.size(); j++) {
            if (trees[i].strings[j].first == templ)
                throw TBadStringException("There is such a string");
        }
    }

    for (size_t i = 0; ; i++) {
        if (trees.size() == i) {
            trees.resize(i + 1);
            trees[i] = cur;
            break;
        } else {
            if (!trees[i].isEmpty()) {
                cur = Merge(cur, trees[i]);
            } else {
                trees[i] = cur;
                break;
            }
        }
    }

    return n++;
}

TMatchResults TDynamicTemplateMatcher::MatchStream(ICharStream &stream) {
    TMatchResults result;
    size_t pref = 0, n = 0;

    if (this -> n == 0)
        throw TNotSupportedException("You need to add at least one template");
    std::vector<Vertex*> cur(trees.size());
    for (size_t i = 0; i < cur.size(); i++) {
        cur[i] = trees[i].root;
    }

    while (!stream.IsEmpty()) {
        n++;
        char c = stream.GetChar();

        if ((unsigned char)c < 32)
        {
            throw TBadStringException("Some chars in stream are not in range 32-255");
        }

        for (size_t i = 0; i < cur.size(); i++) {
            if (!trees[i].isEmpty()) {
                cur[i] = trees[i].go(cur[i], c);
            }
        }

        for (size_t i = 0; i < cur.size(); i++) {
            if (!trees[i].isEmpty()) {
                Vertex* ans = cur[i];

                while(ans != trees[i].root) {
                    if(ans -> leaf != -1)
                        result.push_back(std::make_pair(n, ans -> leaf));
                    ans = trees[i].get_closest(ans);
                }
            }
        }
    }

    return result;
}
