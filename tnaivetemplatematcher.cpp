#include "tnaivetemplatematcher.h"
#include "tbadstringexception.h"

TStringId TNaiveTemplateMatcher::AddTemplate(const std::string &temp) {
    temps.push_back(temp);
    return (int)temps.size() - 1;
}

TMatchResults TNaiveTemplateMatcher::MatchStream(ICharStream &stream) {
    std::string current;
    TMatchResults result;
    while (!stream.IsEmpty()) {
        char c = stream.GetChar();
        current += c;

        if (c < char(32))
            throw TBadStringException("Some chars are not in range 32-255");

        for (size_t i = 0; i < temps.size(); i++) {
            if (current.size() >= temps[i].size()) {
                bool eq = true;
                for (size_t j = 0; j < temps[i].size(); j++) {
                    if (temps[i][j] != current[current.size() - temps[i].size() + j])
                        eq = false;
                }
                if(eq == true) {
                    result.push_back(std::make_pair(current.size(), i));
                }
            }
        }
    }

    return result;
}
