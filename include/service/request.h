#ifndef ARES_SERVICE_REQUEST_H
#define ARES_SERVICE_REQUEST_H

#include <string>

namespace ares {
namespace service {

class Request {
  public:

    virtual ~Request() = default;

    virtual std::string evaluate() = 0;

};

} // namespace service 
} // namespace ares

#endif // ARES_SERVICE_REQUEST_H
