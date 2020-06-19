#include "example/example_reader.h"

namespace ares {
namespace example {

bool ExampleReader::has_metadata() const {
    return false;
}

uint64_t ExampleReader::get_stream_start_time() const {
    return this->start_time;
}

uint64_t ExampleReader::get_stream_end_time() const {
    return this->end_time;
}

void ExampleReader::check_data_source() const {
    if (!has_data_source) {
        throw ares::util::ReadException("Input data source was not "
                                              "initialized");
    }
}

bool ExampleReader::read_line(std::stringstream &source) {
    std::getline(source, latest_read_line);
    return source.good();
}

void ExampleReader::set_background_source(std::string background_string) {
    if (has_background_source) {
        background_source.clear();
    } else {
        has_background_source = true;
    }
    background_source.str(background_string);
}

std::vector<std::string> ExampleReader::read_background_data() {
    std::vector<std::string> result;
    if (has_background_source) {
        bool keep_going = true;
        while (keep_going) {
            if (latest_read_line.empty()) {
                read_line(background_source);
            }
            auto line_string = parse_background_line();
            latest_read_line.clear();
            if (line_string.empty()) {
                keep_going = false;
            } else {
                result.push_back(std::move(line_string));
            }
        }
    }
    return result;
}

void ExampleReader::set_data_source(std::string data_source_string) {
    if (has_data_source) {
        data_source.clear();
    } else {
        has_data_source = true;
    }
    data_source.str(data_source_string);
}

bool ExampleReader::is_valid_line(
        std::string key, std::string delimiter, std::string value) const {
    bool result = !key.empty() && !delimiter.empty() && !value.empty();
    const char delimiter_char = delimiter.front();
    result = result && delimiter_char == EXPECTED_LINE_DELIMITER;
    return result;
}

std::unordered_map<char, std::string> ExampleReader::parse_latest_line() const {
    std::unordered_map<char, std::string> result;
    std::stringstream line_stream(latest_read_line);
    std::string key;
    std::string delimiter;
    std::string value;
    line_stream >> key >> delimiter;
    std::getline(line_stream, value);
    if (!line_stream.fail() && is_valid_line(key, delimiter, value)) {
        result[LINE_KEY] = key;
        result[LINE_DELIMITER] = delimiter;
        result[LINE_VALUE] = value;
    }
    return result;
}

std::string ExampleReader::parse_background_line() const {
    std::string result;
    std::stringstream line_stream(latest_read_line);
    std::string value;
    std::getline(line_stream, value);
    if (!line_stream.fail()) {
        result = std::move(value);
    }
    return result;
}

std::vector<std::string> ExampleReader::read_all_lines() {
    check_data_source();
    std::vector<std::string> result;
    while (read_line(data_source)) {
        auto line_map = parse_latest_line();
        if (!line_map.empty()) {
            result.push_back(line_map.at(LINE_VALUE));
        }
    }
    latest_read_line.clear();
    return result;
}

std::vector<std::string> ExampleReader::read_all_data() {
    return read_all_lines();
}

std::vector<std::string>
ExampleReader::read_next_data(uint64_t request_time_point) {
    check_data_source();
    std::vector<std::string> fact_vector;
    bool keep_going = true;
    while (keep_going) {

        /* When reading a line, the time point will be checked.
         * If the reader has read a line corresponding to the next time point
         * than it can conclude that all the input for the requested time point
         * has been read. Now it can return.
         * The first line in the input corresponding to the next time point,
         * that has just been read, will be kept in the latest_read_line variable
         * until read_next_data() is called again.
         */

        if (latest_read_line.empty()) {
            read_line(data_source);
        }
        auto line_map = parse_latest_line();
        if (line_map.empty()) {
            keep_going = false;
        } else {
            uint64_t input_time = stoull(line_map.at(LINE_KEY));
            if (input_time == request_time_point) {
                fact_vector.push_back(line_map[LINE_VALUE]);
                latest_read_line.clear();
            } else if (input_time > request_time_point) {
                keep_going = false;
            } else {
                // this should never happen, as it is already checked in
                // InputManager and Program. But let's check anyway
                throw ares::util::FormatException(
                        "ExampleReader: out-of-order read. Most likely do to bad "
                        "format of input source");
            }
        }
    }
    return fact_vector;
}

bool ExampleReader::fetch_metadata() {
    check_data_source();
    if (!has_metadata_m) {
        data_source >> start_time;
        data_source >> end_time;
        has_metadata_m = true;
    }
    return has_metadata_m;
}

} // namespace example 
} // namespace ares
