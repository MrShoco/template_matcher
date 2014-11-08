#include <queue>
#include "tnaivetemplatematcher.h"
#include "tbadstringexception.h"

TStringId TNaiveTemplateMatcher::AddTemplate(const std::string &templ) {
    if (templ.size() == 0)
        throw TBadStringException("Empty template");

    for(size_t i = 0; i < templ.size(); i++) {
        if((unsigned char)templ[i] < 32)
            throw TBadStringException("Some chars in template are not in range 32-255");
    }

    templs.push_back(templ);
    max_size = std::max(max_size, templ.size());
    return (int)templs.size() - 1;
}

TMatchResults TNaiveTemplateMatcher::MatchStream(ICharStream &stream) {
    std::string current;
    TMatchResults result;
    size_t n = 0;

    while (!stream.IsEmpty()) {
        n++;
        char c = stream.GetChar();
        current += c;

        if(current.size() > max_size)
            current = current.substr(1, current.size() - 1);

        if ((unsigned char)c < 32)
        {
            throw TBadStringException("Some chars in stream are not in range 32-255");
        }

        for (size_t i = 0; i < templs.size(); i++) {
            if (current.size() >= templs[i].size()) {
                bool eq = true;
                for (size_t j = 0; j < templs[i].size(); j++) {
                    if (templs[i][j] != current[current.size() - templs[i].size() + j])
                        eq = false;
                }
                if(eq == true) {
                    result.push_back(std::make_pair(n, i));
                }
            }
        }
    }

    return result;
}
