//
// Created by mike on 6/15/18.
//
#ifndef LASER_PROGRAM_PROGRAM_H
#define LASER_PROGRAM_PROGRAM_H

#include <vector>
#include <string>
#include <unordered_map>

#include "excetion/request_exception.h"
#include "input/input_manager.h"
#include "output/output_manager.h"
#include "formula/formula.h"
#include "rule/rule.h"
#include "strata.h"

namespace laser {
namespace program {

class Program {
private:

    input::InputManager input_manager;
    output::OutputManager output_manager;

    long long int current_time = 0;
    long long int current_tuple_counter = 0;
    long long int stream_start_time = 0;
    long long int stream_end_time = 0;

    long long int number_of_background_facts = 0;

    Strata strata;
    std::vector<rule::Rule *> rule_vector;

// methods

    bool eval(long long int request_time_point);

    void expire_outdated_groundings(long long int current_time,
                                    long long int current_tuple_counter);

    std::unordered_map<std::string, std::vector<formula::Formula *>>
    get_new_conclusions();

public:

// constructors & destructors

    Program(input::InputManager input_manager,
            output::OutputManager output_manager);

    virtual ~Program();

// getters & setters

// const methods

// methods

    void evaluate(long long int request_time_point);

};

} // namespace program
} // namespace laser



#endif // LASER_PROGRAM_PROGRAM_H
