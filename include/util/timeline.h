#ifndef ARES_UTIL_TIMELINE_H
#define ARES_UTIL_TIMELINE_H

#include <climits>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <mutex>

namespace ares::util {

class Timeline {
  private:
    uint64_t time = 0;
    uint64_t tuple_count = 0;
    uint64_t min_time = 0;
    uint64_t max_time = ULLONG_MAX;
    std::shared_ptr<std::mutex> history_mutex;

    /** records the cumulated tuple counter at each timepoint
     * key: timepoint
     * value: tuple counter
     */
    std::shared_ptr<std::unordered_map<uint64_t, uint64_t>> tuple_count_history;

  public:
    static const uint64_t INFINITE_TIME;

    Timeline();

    uint64_t get_time() const;
    void set_time(uint64_t time);
    void set_start_time(uint64_t initial_time);
    void increment_time();
    void decrement_time();

    void set_tuple_count(uint64_t tuple_count);

    uint64_t get_min_time() const;
    void set_min_time(uint64_t min_time);

    uint64_t get_max_time() const;
    void set_max_time(uint64_t max_time);

    // uint64_t get_min_tuple_count() const;
    // void set_min_tuple_count(uint64_t min_tuple_count);

    // uint64_t get_max_tuple_count() const;
    // void set_max_tuple_count(uint64_t max_tuple_count);

    bool is_past_max_time() const;

    bool is_past_max_tuple_count() const;

    /**
     * Returns (initial_value - substracted_value)
     * Functions like this might be necesary in case timeline has more
     * timepoints than the limit of uint64_t.
     */
    uint64_t substract(uint64_t initial_value, uint64_t substracted_val) const;

    uint64_t min(uint64_t first, uint64_t second) const;

    uint64_t max(uint64_t first, uint64_t second) const;

    uint64_t get_tuple_count_at(uint64_t timepoint) const;
};

// TODO: when some time == ULLONG_MAX it should flip to zero. We should check
// for this when we increment/decrement/set
// TODO: Then we should implement a `compare` function that will be able to
// detect that 1 > ULLONG max, since in this case we can be sure there was a
// flip
// TODO: Limits don't have to be set. They can be infinite. In this case, we
// should have functions: set_infinite_time(), set_inifinite_tuples(), that set
// some private bool values. When this vals are set, get_end_time() should
// always return ULLONG_MAX, or something.

} // namespace util::ares

#endif // ARES_UTIL_TIMELINE_H
