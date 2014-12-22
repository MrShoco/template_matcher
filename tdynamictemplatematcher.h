#ifndef T_DYNAMIC_TEMPLATE_MATCHER
#define T_DYNAMIC_TEMPLATE_MATCHER

#include <vector>
#include <iostream>
#include <utility>
#include "imetatemplatematcher.h"
class TDynamicTemplateMatcher : public IMetaTemplateMatcher {
    struct Vertex {
        Vertex(Vertex* p_, char c_): leaf(-1), p(p_), link(nullptr), closest(NULL), pch(c_) {
            for (int i = 0; i < 256; i++)
                next[i] = go[i] = nullptr;
            id = ida++;
        }
        ~Vertex() {
            for(size_t i = 0; i < 256; i++)
                if(next[i] != nullptr)
                    delete next[i];
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

    struct Tree {
        Tree() {
            root = new Vertex(root, 0);
        }
        void clear() {
            delete root;
            root = nullptr;
            strings.clear();
        }
        Vertex *root;

        bool isEmpty() {
            return root == nullptr;
        }

        int add(const std::string &s, int n) {
            strings.push_back(make_pair(s, n));
            Vertex* cur = root;
            for(size_t i = 0; i < s.size(); i++) {
                if(cur -> next[s[i]] == nullptr) {
                    cur -> next[s[i]] = new Vertex(cur, s[i]);
                }
                Vertex* next = cur -> next[s[i]];

                cur = next;
            }
            cur -> leaf = n++;
            return n - 1;
        }
        
        Vertex* get_closest(Vertex* cur) {
            if (cur == root)
                return cur;

            if (cur -> closest != nullptr)
                return cur -> closest;

            if (get_link(cur) -> leaf != -1)
                cur -> closest = get_link(cur);
            else
                cur -> closest = get_closest(get_link(cur));

            return cur -> closest;
        }

        Vertex* get_link(Vertex* cur) {
            if (cur -> link == nullptr)
                cur -> link = (cur -> p == root) ? root : go(get_link(cur -> p), cur -> pch);

            return cur -> link;
        }

        Vertex* go(Vertex* cur, char c) {
            if (cur -> go[c] == nullptr) {
                if (cur -> next[c] != nullptr)
                    cur -> go[c] = cur -> next[c];
                else
                    cur -> go[c] = (cur == root) ? root : go(get_link(cur), c);
            }

            return cur -> go[c];
        }

        std::vector<std::pair<std::string, int>> strings;
    };

public:
    TStringId AddTemplate(const std::string &temp);
    TMatchResults MatchStream(ICharStream &stream);
private:
    Tree Merge(Tree &a, Tree &b) {
        Tree c;
        for(size_t i = 0; i < a.strings.size(); i++)
            c.add(a.strings[i].first, a.strings[i].second);
        for(size_t i = 0; i < b.strings.size(); i++)
            c.add(b.strings[i].first, b.strings[i].second);
        a.clear();
        b.clear();
        return c;
    }
    Tree Create(const std::string &temp, int n) {
        Tree a;
        a.add(temp, n);
        return a;
    }
    int n = 0;
    static int ida;
    std::vector<Tree> trees;

};

#endif
