#ifndef ARES_UTIL_READ_EXCEPTION_H
#define ARES_UTIL_READ_EXCEPTION_H

#include <exception>

namespace ares {
namespace util {


class ReadException : public std::exception {
private:
    char const *message_m = "ReadException";
public:
    ReadException() = default;

    explicit ReadException(char const *message) : message_m(message) {}

    const char *what() const noexcept override {
        return message_m;
    }
};

} // namespace util
} // namespace ares


#endif // ARES_UTIL_READ_EXCEPTION_H
