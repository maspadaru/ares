#ifndef ARES_UTIL_SETTINGS_H
#define ARES_UTIL_SETTINGS_H

#include "util/chase_algorithm.h"

namespace ares {
namespace util {

class Settings {
  private:
    ChaseAlgorithm chase_algorithm = ChaseAlgorithm::OBLIVIOUS;  
    bool has_global_null_values_m = true;

    Settings() {} 

  public:
    Settings(Settings const&) = delete;

    void operator=(Settings const&) = delete;

    static Settings &get_instance();

    ChaseAlgorithm get_chase_algorithm() const;

    bool has_global_null_values() const;

    void set_global_null_values(bool has_global_nulls);

    void set_chase_algorithm(ChaseAlgorithm chase_algorithm);
};

} // namespace util
} // namespace ares

#endif // ARES_UTIL_SETTINGS_H
