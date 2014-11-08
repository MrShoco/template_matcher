#ifndef STRING_STREAM
#define STRING_STREAM

#include "icharstream.h"
#include <iostream>

class StringStream : public ICharStream {
public:
    StringStream(std::string stream): stream_(stream), ind(0) {}
    StringStream(): stream_(""), ind(0) {}

    bool IsEmpty() const;

    char GetChar();

    void AddChar(char c);

    void Reset();

private:
    size_t ind;
    std::string stream_;
};

#endif
