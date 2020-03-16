#ifndef ARES_EXAMPLE_EXAMPLE_READER_H
#define ARES_EXAMPLE_EXAMPLE_READER_H

#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <util/format_exception.h>
#include <util/read_exception.h>

namespace ares {
namespace example {

class ExampleReader {
  private:
    static constexpr char LINE_KEY = 0;
    static constexpr char LINE_DELIMITER = 1;
    static constexpr char LINE_VALUE = 2;
    static constexpr char EXPECTED_LINE_DELIMITER = ':';

    std::stringstream source;
    bool has_source = false;
    uint64_t start_time;
    uint64_t end_time;
    bool has_metadata_m = false;
    std::string latest_read_line;

    void check_source() const;

    bool read_line();

    std::unordered_map<char, std::string> parse_latest_line() const;

    bool is_valid_line(std::string key, std::string delimiter,
                       std::string value) const;

    std::vector<std::string> read_all_lines();

  public:
    ~ExampleReader() = default;

    bool has_metadata() const;

    bool fetch_metadata();

    uint64_t get_stream_start_time() const;

    uint64_t get_stream_end_time() const;

    std::vector<std::string> read_all_data();

    std::vector<std::string> read_next_data(uint64_t request_time_point);

    void set_source(std::string source_string);
};

} // namespace example
} // namespace ares

#endif // ARES_EXAMPLE_EXAMPLE_READER_H
