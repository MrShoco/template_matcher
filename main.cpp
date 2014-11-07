#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>
#include "icharstream.h"
#include "tnaivetemplatematcher.h"

class StringStream : public ICharStream {
public:
    StringStream(std::string stream): stream_(stream), ind(0) {}

    bool IsEmpty() const {
        return ind >= stream_.size();
    }

    char GetChar() {
        if(IsEmpty())
            throw std::out_of_range("Stream ends");
        return stream_[ind++];
    }

    void AddChar(char c) {
        stream_ += c;
    }

private:
    size_t ind;
    std::string stream_;
};

int main()
{
    try {
        StringStream str("abacaba");
        
        TNaiveTemplateMatcher matcher;
        matcher.AddTemplate("aba");
        matcher.AddTemplate("bacaba");
        matcher.AddTemplate("abacaba");
        matcher.AddTemplate("a");
        
        std::vector<std::pair<size_t, int> > matched = matcher.MatchStream(str);

        for (int i = 0; i < matched.size(); i++) {
            std::cout << matched[i].first << " " << matched[i].second << std::endl;
        }
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
