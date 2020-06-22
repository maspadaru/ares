#include <iostream>

#include <acyclicity/naive_smfa.h>
#include <core/reasoner.h>
#include <example/example_io_manager.h>
#include <rule/rule_parser.h>
#include <util/chase_algorithm.h>

void run(std::string const &name, std::string const &stream_string,
         std::string const &background_string,
         std::string const &rule_string,
         ares::util::ChaseAlgorithm chase_algorithm) {
    std::cout << std::endl;
    std::cout << " Test: " << name << std::endl;
    std::cout << " =================================== " << std::endl;
    // It might not be very intuitive, but chase_algorithm needs to be set
    // before parsing the rules !!!
    // As a general rule set the settings before initializing anything else.
    // I should make this more obvious somehow...
    ares::util::Settings::get_instance().set_chase_algorithm(chase_algorithm);
    auto example_io_manager = 
        ares::example::ExampleIOManager(stream_string, background_string);
    auto rule_parser = ares::rule::RuleParser(rule_string);
    auto rule_vector = rule_parser.get_rules();
    auto reasoner = ares::core::Reasoner(rule_vector, &example_io_manager);
    reasoner.start();
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}

void run_acyclicity_test(std::string const &name,
                         std::string const &rule_string, bool expected) {
    bool acyclicity_result;
    std::cout << std::endl;
    std::cout << " Acyclicity Test: " << name << std::endl;
    ares::util::Settings::get_instance().set_chase_algorithm(
            ares::util::ChaseAlgorithm::SKOLEM);
    auto rule_parser = ares::rule::RuleParser(rule_string);
    auto rule_vector = rule_parser.get_rules();
    auto naive_smfa = ares::acyclicity::NaiveSMFA(rule_vector);
    acyclicity_result = naive_smfa.has_terminating_chase();
    std::cout << " Naive_SMFA returned: " << acyclicity_result
              << " expected: " << expected << ";" << std::endl;
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl;
}

void test_acyclicity_simple() {
    const std::string name = "Acyclicity Simple";
    std::string rule_string = "p(a, Z, b, X, Z) := q(X, Y, Z)\n";
    bool expected_has_terminating_chase = true;
    run_acyclicity_test(name, rule_string, expected_has_terminating_chase);
}

void test_acyclicity_obvious_cycle() {
    const std::string name = "Acyclicity Obvious Cycle";
    std::string rule_string = "p(X) := r(X)\n"
                              "q(Y) && m(X, Y) := p(X)\n"
                              "p(Y) && n(X, Y) := q(X)\n";
    bool expected_has_terminating_chase = false;
    run_acyclicity_test(name, rule_string, expected_has_terminating_chase);
}

void test_acyclicity_obvious_cycle_with_time() {
    const std::string name = "Acyclicity Obvious Cycle With Time";
    std::string rule_string = "p(X) := [$, 3][B]r(X)\n"
                              "q(Y) && m(X, Y) := [$, 5][D]p(X)\n"
                              "p(Y) && n(X, Y) := [$, 5][D]q(X)\n";
    bool expected_has_terminating_chase = false;
    run_acyclicity_test(name, rule_string, expected_has_terminating_chase);
}

void test_acyclicity_only_first_timepoint() {
    // In this test, a cycle will only occur at the first timepoint
    const std::string name = "Acyclicity Only First Timepoint";
    std::string rule_string = "p(X) := r(X)\n"
                              "q(Y) && m(X, Y) := p(X)\n"
                              "p(Y) && n(X, Y) := [$, 3][B]q(X)\n";
    bool expected_has_terminating_chase = false;
    run_acyclicity_test(name, rule_string, expected_has_terminating_chase);
}

void test_run() {
    const std::string name = "Test Run";
    std::string background_string = "";
    std::string stream_string = "1 4 "
                                "1 : <http://www.w3.org/2000/01/rdf-schema#label>(<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Book>, \"book1\")\n"
                                "2 : <http://www.w3.org/2000/01/rdf-schema#label>(<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Book>, \"book2\")\n"
                                "3 : <http://www.w3.org/2000/01/rdf-schema#label>(<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Book>, \"book3\")\n"
                                "4 : \n";
    std::string rule_string = " <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>(Y, X) := <http://www.w3.org/2000/01/rdf-schema#label>(X, Y)\n";
    auto chase_alg = ares::util::ChaseAlgorithm::OBLIVIOUS;
    run(name, stream_string, background_string, rule_string, chase_alg);
}

int main() {
    test_run();
    // test_acyclicity_simple();
    // test_acyclicity_obvious_cycle();
    // test_acyclicity_obvious_cycle_with_time();
    // test_acyclicity_only_first_timepoint();
    exit(0);
}
