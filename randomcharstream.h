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
        rand_seed = rand();
        srand(rand_seed);
        if (rmn >= rmx)
        {
            throw std::range_error("Bad Ranges in Random Stream");
        }
        range_max = rmx;
        range_min = rmn;
        ind = 0;
    }

    bool IsEmpty() const;

    char GetChar();

    void Reset();
private:
    size_t sz_;
    size_t ind;
    int range_min, range_max;
    int rand_seed;
};

#endif
