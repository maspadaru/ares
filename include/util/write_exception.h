#ifndef ARES_UTIL_WRITE_EXCEPTION_H
#define ARES_UTIL_WRITE_EXCEPTION_H

#include <exception>

namespace laser {
namespace util {


class WriteException : public std::exception {
private:
    char const *message_m = "WriteException";
public:
    WriteException() = default;

    explicit WriteException(char const *message) : message_m(message) {}

    const char *what() const noexcept override {
        return message_m;
    }
};

} // namespace util
} // namespace laser


#endif // ARES_UTIL_WRITE_EXCEPTION_H
