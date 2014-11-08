#ifndef RANDOM_CHAR_STREAM
#define RANDOM_CHAR_STREAM

#include "icharstream.h"
#include <iostream>
#include <exception>
#include <stdexcept>
#include <ctime>

class RandomCharStream : public ICharStream {
public:
    RandomCharStream(size_t sz = 0, unsigned char rmn = 32, unsigned char rmx = 255): sz_(sz) {
        srand(time(0));
        if (rmn >= rmx)
        {
            throw std::range_error("Bad Ranges in Random Stream");
        }
        range_max = rmx;
        range_min = rmn;
    }

    bool IsEmpty() const;

    char GetChar();
private:
    size_t sz_;
    int range_min, range_max;
};

#endif
