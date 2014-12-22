#ifndef T_STATIC_TEMPLATE_MATCHER
#define T_STATIC_TEMPLATE_MATCHER

#include <vector>
#include <iostream>
#include "imetatemplatematcher.h"

class TStaticTemplateMatcher : public IMetaTemplateMatcher {
    struct Vertex {
        Vertex(Vertex* p_, char c_): leaf(-1), p(p_), link(NULL), closest(NULL), pch(c_) {
            for (int i = 0; i < 256; i++)
                next[i] = go[i] = NULL;
            id = ida++;
        }

        Vertex* p;
        char pch;
        Vertex* link;
        Vertex* closest;
        Vertex* go[256];
        Vertex* next[256];
        int leaf;
        int id;
    };

public:
    TStringId AddTemplate(const std::string &temp);
    TMatchResults MatchStream(ICharStream &stream);
private:
    int n = 0;
    static int ida;
    Vertex* root;
    bool matched = false;

    int add(const std::string &s) {
        Vertex* cur = root;
        for(size_t i = 0; i < s.size(); i++) {
            if(cur -> next[s[i]] == NULL) {
                cur -> next[s[i]] = new Vertex(cur, s[i]);
            }
            Vertex* next = cur -> next[s[i]];

            cur = next;
        }
        if (cur -> leaf == -1)
            cur -> leaf = n++;
        return cur -> leaf;
    }
    
    Vertex* get_closest(Vertex* cur) {
        if (cur == root)
            return cur;

        if (cur -> closest != NULL)
            return cur -> closest;

        if (get_link(cur) -> leaf != -1)
            cur -> closest = get_link(cur);
        else
            cur -> closest = get_closest(get_link(cur));

        return cur -> closest;
    }

    Vertex* get_link(Vertex* cur) {
        if (cur -> link == NULL)
            cur -> link = (cur -> p == root) ? root : go(get_link(cur -> p), cur -> pch);

        return cur -> link;
    }

    Vertex* go(Vertex* cur, char c) {
        if (cur -> go[c] == NULL) {
            if (cur -> next[c] != NULL)
                cur -> go[c] = cur -> next[c];
            else
                cur -> go[c] = (cur == root) ? root : go(get_link(cur), c);
        }

        return cur -> go[c];
    }
};

#endif
