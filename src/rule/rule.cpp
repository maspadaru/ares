//
// Created by mike on 6/15/18.
//

#include <utility>
#include "rule/rule.h"

namespace laser {
namespace rule {


// constructors & destructors



Rule::Rule(
        formula::Formula *head_formula,
        formula::Formula *body_formula) :
        head(head_formula->clone()),
        body(body_formula->clone()) {
    compute_variable_map(); 
}


Rule::~Rule() {
    delete &head;
    delete &body;
}

Rule::Rule(Rule const &other) : head(other.head.clone()), body(other.body.clone()) {
    variable_map = other.variable_map;
}

Rule::Rule(Rule &&other) noexcept  : head(other.head.move()),
        body(other.body.move()) {
    variable_map = std::move(other.variable_map);
}

Rule &Rule::operator=(Rule const &other) {
    this->head = other.head.clone();
    this->body = other.body.clone();
    this->variable_map = other.variable_map;
    return *this;
}

Rule &Rule::operator=(Rule &&other) noexcept {
    this->head = other.head.move();
    this->body = other.body.move();
    variable_map = std::move(other.variable_map);
    return *this;
}

// getters & setters

formula::Formula const& Rule::get_head() {
    return this->head;
}

// methods

void Rule::expire_outdated_groundings(
        uint64_t current_time,
        uint64_t current_tuple_counter) {
    head.expire_outdated_groundings(current_time, current_tuple_counter);
    body.expire_outdated_groundings(current_time, current_tuple_counter);

}

void Rule::debug_print() const {
    std::cerr << std::endl;
    std::cerr << "Rule -> head: ";
    head.debug_print();
    std::cerr <<  "Rule body:" ;
    body.debug_print();
    std::cerr << "// Rule " << std::endl;
    std::cerr << std::endl;

}

bool Rule::evaluate(
        uint64_t current_time, uint64_t current_tuple_counter,
        std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    return body.evaluate(current_time, current_tuple_counter, facts);
}

void Rule::compute_variable_map() {
    auto head_variable_vector = head.get_variable_names(); 
    for (int head_index = 0; head_index < head_variable_vector.size(); head_index++) {
        auto body_index = body.get_variable_index(head_variable_vector.at(head_index));
        variable_map.insert({head_index, body_index}); 
    }
}

formula::Grounding Rule::convert_to_head_grounding(formula::Grounding grounding) {
    auto result = formula::Grounding(grounding.get_consideration_time(),
            grounding.get_horizon_time(), grounding.get_consideration_count(), 
            grounding.get_horizon_count());
    std::vector<std::string> result_vector;
    for (size_t head_index = 0; head_index < result_vector.size(); head_index++) {
        result_vector.push_back(grounding.get_substitution(head_index));
    }
    result.add_substitution_vector(result_vector);
    return result;
}

bool Rule::derive_conclusions(
        uint64_t current_time, uint64_t current_tuple_counter) {
    std::vector<formula::Grounding> predicate_facts;
    std::vector<formula::Grounding> body_groundings = body.get_groundings();
    for (auto grounding : body_groundings) {
         auto head_grounding = convert_to_head_grounding(grounding);
         predicate_facts.push_back(head_grounding);
    }
    std::unordered_map<std::string, std::vector<formula::Grounding>> body_facts;
    // a bit overcomplicated, but this will allow more flexibility in the head
    for (auto predicate : head.get_predicate_vector()) {
        body_facts.insert({predicate, predicate_facts});
    }
    return head.evaluate(current_time, current_tuple_counter, body_facts);
}




} // namespace rule
} // namespace laser
