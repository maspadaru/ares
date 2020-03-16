#ifndef ARES_RULE_SHARED_H
#define ARES_RULE_SHARED_H

#include <cstdint>
#include <memory>
#include <map>
#include <unordered_map>

#include "util/database.h"
#include "util/grounding.h"
#include "util/timeline.h"

namespace ares {
namespace rule {
namespace shared {

std::unordered_map<std::string, int>
make_index(std::vector<std::string> const &vector);

std::map<std::string, int>
make_ordered_index(std::vector<std::string> const &vector);

} // namespace shared
} // namespace rule
} // namespace ares

#endif // ARES_RULE_SHARED_H
