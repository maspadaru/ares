#ifndef ARES_UTIL_FORMAT_EXCEPTION_H
#define ARES_UTIL_FORMAT_EXCEPTION_H

#include <exception>

namespace ares {
namespace util {


class FormatException : public std::exception {
private:
    char const *message_m = "FormatException";
public:
    FormatException() = default;

    explicit FormatException(char const *message) : message_m(message) {}

    const char *what() const noexcept override {
        return message_m;
    }
};

} // namespace util
} // namespace ares


#endif // ARES_UTIL_FORMAT_EXCEPTION_H
