#ifndef T_BAD_STRING_EXCEPTION
#define T_BAD_STRING_EXCEPTION

#include <exception>

class TBadStringException: public std::exception {
public:
    TBadStringException(std::string msg): msg_(msg) {}
    ~TBadStringException() throw() {}
    virtual const char* what() const throw() {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

#endif
