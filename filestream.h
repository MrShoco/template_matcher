#ifndef FILE_STREAM
#define FILE_STREAM

#include "icharstream.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

class FileStream : public ICharStream {
public:
    FileStream(std::string file_name = "") {
        fin = new std::ifstream(file_name);
    }

    bool IsEmpty() const;

    char GetChar();

    void Reset();

private:
    std::ifstream *fin;
};

#endif
