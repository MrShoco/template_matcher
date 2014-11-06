#ifndef I_CHAR_STREAM
#define I_CHAR_STREAM

class ICharStream {
public:
    virtual char GetChar();
    virtual bool IsEmpty() const;
    virtual ~ICharStream();
};

#endif
