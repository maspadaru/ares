#include "util/global.h"

namespace ares::util {

Global &Global::get_instance() {
    static Global instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
    return instance;
}


uint64_t Global::get_null_value_count() {
    this->null_value_count++;
    return this->null_value_count;
}

} // namespace ares::util
