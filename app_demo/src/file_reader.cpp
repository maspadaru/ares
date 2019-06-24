// app_demo file_reader.cpp

#include "file_reader.h"

FileReader::FileReader(std::string const &stream_path, int facts_per_timepoint)
    : facts_per_timepoint(facts_per_timepoint) {
    in.open(stream_path);
}

FileReader::~FileReader() { in.close(); }

bool FileReader::read_line(std::string &line) {
    std::getline(in, line);
    return in.good();
}

std::vector<std::string> FileReader::read_all_data() {
    std::vector<std::string> result;
    std::string line;
    while (read_line(line)) {
        result.push_back(line);
    }
    return result;
}

std::vector<std::string>
FileReader::read_next_data(uint64_t request_time_point) {
    std::vector<std::string> fact_vector;
    std::string line;
    while (read_line(line)) {
        fact_vector.push_back(line);
    }
    return fact_vector;
}
