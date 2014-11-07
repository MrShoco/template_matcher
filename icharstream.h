#ifndef I_CHAR_STREAM
#define I_CHAR_STREAM

class ICharStream {
public:
    virtual char GetChar() = 0;
    virtual bool IsEmpty() const = 0;
    virtual ~ICharStream() = default;
};

#endif
