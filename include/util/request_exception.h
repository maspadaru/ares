#ifndef ARES_UTIL_REQUEST_EXCEPTION_H
#define ARES_UTIL_REQUEST_EXCEPTION_H

#include <exception>

namespace laser {
namespace util {


class RequestException : public std::exception {
private:
    char const *message_m = "RequestException";
public:
    RequestException() = default;

    explicit RequestException(char const *message) : message_m(message) {}

    const char *what() const noexcept override {
        return message_m;
    }
};

} // namespace util
} // namespace laser


#endif // ARES_UTIL_REQUEST_EXCEPTION_H
