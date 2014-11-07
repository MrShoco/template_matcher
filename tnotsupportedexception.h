#ifndef T_NOT_SUPPORTED_EXCEPTION
#define T_NOT_SUPPORTED_EXCEPTION

#include <exception>

class TNotSupportedException: public std::exception {
public:
    TNotSupportedException(std::string msg): msg_(msg) {}
    ~TNotSupportedException() throw() {}
    virtual const char* what() const throw() {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

#endif
