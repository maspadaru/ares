#include "rule/shared.h"

namespace ares {
namespace rule {
namespace shared {

std::unordered_map<std::string, int>
make_index(std::vector<std::string> const &vector) {
    std::unordered_map<std::string, int> result;
    size_t index = 0;
    for (auto const &var_name : vector) {
        result.try_emplace(var_name, index);
        index++;
    }
    return result;
}

std::map<std::string, int>
make_ordered_index(std::vector<std::string> const &vector) {
    std::map<std::string, int> result;
    size_t index = 0;
    for (auto const &var_name : vector) {
        result.try_emplace(var_name, index);
        index++;
    }
    return result;
}

} // namespace share
} // namespace rule
} // namespace ares
