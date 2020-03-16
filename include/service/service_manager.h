#ifndef ARES_SERVICE_SERVICE_MANAGER_H
#define ARES_SERVICE_SERVICE_MANAGER_H

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "service/query.h"
#include "service/request.h"
#include "service/request_parser.h"
#include "service/request_type.h"

namespace ares {
namespace service {

class ServiceManager {
  private:
    std::vector<std::shared_ptr<util::Grounding>> database_facts;
    uint64_t time = 0;
    std::string request_string;
    std::istream &in;
    std::ostream &out;

    bool read_request();

    bool read_line(std::string &line);

  public:
    explicit ServiceManager();
    explicit ServiceManager(std::istream &input, std::ostream &output);

    virtual ~ServiceManager() = default;

    void update(uint64_t time,
                std::vector<std::shared_ptr<util::Grounding>> facts,
                std::vector<std::shared_ptr<util::Grounding>> conclusions);

    void serve_requests();
};

} // namespace service
} // namespace ares

#endif // ARES_SERVICE_SERVICE_MANAGER_H
