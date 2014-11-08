#include "tsingletemplatematcher.h"
#include "tbadstringexception.h"
#include "tnotsupportedexception.h"

TStringId TSingleTemplateMatcher::AddTemplate(const std::string &templ) {
    if (!templ_.empty())
        throw TNotSupportedException("You can't add more than one template");

    if (templ.size() == 0)
        throw TBadStringException("Empty template");

    for (size_t i = 0; i < templ.size(); i++) {
        if((unsigned char)templ[i] < 32)
            throw TBadStringException("Some chars in template are not in range 32-255");
    }

    templ_ = templ;

    kmp.resize(templ_.size());
    kmp[0] = 0;

    for (size_t i = 1; i < templ_.size(); i++) {
        size_t j = kmp[i];
        while (j > 0 && templ_[i] != templ_[j])
            j = kmp[j - 1];
        if (templ_[i] == templ_[j])
            j++;
        kmp[i] = j;
    }

    return 0;
}

TMatchResults TSingleTemplateMatcher::MatchStream(ICharStream &stream) {
    std::string current;
    TMatchResults result;
    size_t pref = 0, n = 0;

    while (!stream.IsEmpty()) {
        n++;
        char c = stream.GetChar();
        current += c;

        if ((unsigned char)c < 32)
        {
            throw TBadStringException("Some chars in stream are not in range 32-255");
        }
        
        while (pref > 0 && c != templ_[pref])
            pref = kmp[pref - 1];
        if (c == templ_[pref])
            pref++;

        if (pref == templ_.size())
            result.push_back(std::make_pair(n, 0));
    }

    return result;
}
