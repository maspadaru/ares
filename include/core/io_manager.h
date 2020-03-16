#ifndef ARES_CORE_IO_MANAGER_H
#define ARES_CORE_IO_MANAGER_H

#include <memory>
#include <vector>

#include <util/grounding.h>
#include <util/timeline.h>

namespace ares {
namespace core {

class IOManager {
  public:

    virtual ~IOManager() = default;

    virtual uint64_t read_stream_start_time() = 0;

    virtual uint64_t read_stream_end_time() = 0;

    virtual std::vector<std::shared_ptr<ares::util::Grounding>>
    read_stream_data(ares::util::Timeline &timeline) = 0;

    virtual std::vector<std::shared_ptr<ares::util::Grounding>>
    read_background_data() = 0;

    virtual void
    write_output_data(uint64_t time,
                      std::vector<std::shared_ptr<ares::util::Grounding>>
                          output_vector) = 0;
};

} // namespace core 
} // namespace ares

#endif // ARES_CORE_IO_MANAGER_H
