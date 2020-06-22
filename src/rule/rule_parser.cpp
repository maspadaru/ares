#include "rule/rule_parser.h"

namespace ares::rule {

RuleParser::RuleParser(std::string rule_string)
    : rule_string(std::move(rule_string)) {}

inline void RuleParser::syntax_error(std::string const &error_message) const {
    std::string message = "Line: " + std::to_string(line_counter) +
                          " -> Syntax Error: " + error_message;
    const char *exception_message = message.c_str();
    throw util::FormatException(exception_message);
}

void RuleParser::skip_spaces() {
    input >> std::ws; // eats-up any leading whitespaces.
}

void RuleParser::skip_next_char() { input.get(); }
char RuleParser::read_next_char() { return input.get(); }

void RuleParser::skip_expected_char(char c) {
    char input_char = read_next_char();
    if (input_char != c) {
        std::stringstream error_stream;
        error_stream << "Expected character `" << c
                     << "` but actual caracter was `" << input_char << "`.";
        std::string error_message = error_stream.str();
        syntax_error(error_message);
    }
}

void RuleParser::skip_conjunction_operator() {
    skip_expected_char('&');
    skip_expected_char('&');
}

char RuleParser::peek_next_char() { return input.peek(); }

bool RuleParser::is_next_char_letter() {
    return is_letter_or_sign(peek_next_char()) > 0;
}

bool RuleParser::is_next_char_digit() {
    return std::isdigit(peek_next_char()) > 0;
}

bool RuleParser::is_next_char_letter_or_digit() {
    return is_next_char_letter() || is_next_char_digit();
}

bool RuleParser::is_next_char(char c) { return peek_next_char() == c; }

bool RuleParser::is_next_char_binary_operator() {
    return is_next_char('&') || is_next_char('|');
}

bool RuleParser::is_next_char_conjunction_operator() {
    return is_next_char('&');
}

bool RuleParser::is_next_char_algebra_operator() {
    return is_next_char('+') || is_next_char('-') || is_next_char('*') ||
           is_next_char('/');
}

bool RuleParser::is_next_char_equals_sign() { return is_next_char('='); }

bool RuleParser::is_next_char_condition_operator() {
    return is_next_char('?');
}

void RuleParser::parse_eoln() {
    skip_spaces();
    if (!input.eof()) {
        syntax_error("Expecting EOL");
    }
}

std::string RuleParser::parse_assignment_operator() {
    std::stringstream result_stream;
    result_stream << read_next_char();
    return result_stream.str();
}

std::string RuleParser::parse_algebra_operator() {
    std::stringstream result_stream;
    result_stream << read_next_char();
    return result_stream.str();
}

std::string RuleParser::parse_condition_operator() {
    std::stringstream result_stream;
    read_next_char(); // skipping '?' character
    result_stream << read_next_char();
    if (is_next_char_equals_sign()) {
        result_stream << read_next_char();
    }
    auto result = result_stream.str();
    if (result == "=") {
        // keeping "?=" in this case, to diferentiate fro assignment predicate
        result = "?=";
    }
    return result;
}

char RuleParser::parse_nonzero() {
    char result = read_next_char();
    if (std::isdigit(result) == 0 || result == '0') {
        syntax_error("Expected non-zero digit");
    }
    return result;
}

std::string RuleParser::parse_zero() {
    skip_next_char();
    return "0";
}

char RuleParser::parse_diggit() {
    char result = read_next_char();
    if (std::isdigit(result) == 0) {
        syntax_error("Expected digit");
    }
    return result;
}

bool RuleParser::is_letter_or_sign(char c) {
    return std::isalpha(c) || c=='<' || c=='>'
         || c=='/' || c==':' || c=='"' || c=='#'
         || c=='-' || c=='_' || c=='~' || c=='.'; 
}

char RuleParser::parse_letter() {
    char input_char = read_next_char();
    if (is_letter_or_sign(input_char) == 0) {
        std::stringstream error_stream;
        error_stream << "Expected letter "
                     << " but actual caracter was [" << input_char
                     << "], hex=" << std::hex << (uint16_t)input_char << ".";
        std::string error_message = error_stream.str();
        syntax_error(error_message);
    }
    return input_char;
}

std::string RuleParser::parse_positive_number() {
    std::stringstream result_stream;
    result_stream << parse_nonzero();
    while (is_next_char_digit()) {
        result_stream << read_next_char();
    }
    return result_stream.str();
}

std::string RuleParser::parse_natural_number() {
    if (is_next_char('0')) {
        return parse_zero();
    }
    return parse_positive_number();
}

std::string RuleParser::parse_integer() {
    if (is_next_char('0')) {
        return parse_zero();
    }
    std::stringstream result_stream;
    if (is_next_char('-')) {
        result_stream << read_next_char();
        skip_spaces();
    }
    result_stream << parse_positive_number();
    return result_stream.str();
}

std::string RuleParser::parse_float() {
    std::stringstream result_stream;
    result_stream << parse_integer();
    if (is_next_char('.')) {
        skip_next_char();
        do {
            result_stream << parse_diggit();
        } while (is_next_char_digit());
    }
    return result_stream.str();
}

std::string RuleParser::parse_identifier() {
    std::stringstream result_stream;
    result_stream << parse_letter();
    while (is_next_char_letter_or_digit()) {
        result_stream << read_next_char();
    }
    return result_stream.str();
}

std::string RuleParser::parse_argument() {
    if (is_next_char_letter()) {
        return parse_identifier();
    }
    return parse_integer();
}

std::string RuleParser::parse_float_argument() {
    if (is_next_char_letter()) {
        return parse_identifier();
    }
    return parse_float();
}

std::vector<std::unique_ptr<ares::rule::Rule>> RuleParser::parse_program() {
    std::vector<std::unique_ptr<ares::rule::Rule>> result;
    std::stringstream program_stream;
    program_stream.str(rule_string);
    std::string line;
    line_counter = 0;
    while (std::getline(program_stream, line, '\n')) {
        line_counter++;
        input.clear();
        input.str(line);
        auto rule = parse_rule();
        result.push_back(std::move(rule));
    }
    return result;
}

std::unique_ptr<ares::rule::Rule> RuleParser::parse_rule() {
    inertia_variables.clear();
    auto head = parse_head();
    skip_spaces();
    skip_expected_char(':');
    skip_expected_char('=');
    auto body = parse_body();
    parse_eoln();
    auto result = std::make_unique<ares::rule::Rule>(
        std::move(body), std::move(head), std::move(inertia_variables));
    return std::move(result);
}

std::vector<std::unique_ptr<ares::formula::Formula>> RuleParser::parse_head() {
    return parse_formula_vector();
}
std::unique_ptr<ares::formula::Formula>
RuleParser::parse_extended_head_atom() {
    std::unique_ptr<ares::formula::Formula> result;
    if (is_next_char('@')) {
        result = parse_time_reference();
    } else if (is_next_char('I')) {
        result = parse_inertia_operator();
    }
    return result;
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_head_atom() {
    std::unique_ptr<ares::formula::Formula> result;
    skip_spaces();
    if (is_next_char('[')) {
        skip_next_char();
        skip_spaces();
        result = parse_extended_head_atom();
    } else {
        skip_spaces();
        result = parse_predicate_atom();
    }
    return result;
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_body() {
    std::unique_ptr<ares::formula::Formula> result;
    skip_spaces();
    if (is_next_char('(')) {
        result = parse_complex_formula();
    } else {
        result = parse_binary_formula();
    }
    return result;
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_complex_formula() {
    skip_expected_char('(');
    auto result = parse_binary_formula();
    skip_spaces();
    skip_expected_char(')');
    return result;
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_binary_formula() {
    auto left_term = parse_term();
    skip_spaces();
    while (is_next_char_binary_operator()) {
        auto binary_formula = parse_binary_operator();
        auto right_term = parse_term();
        binary_formula->add_child(std::move(left_term));
        binary_formula->add_child(std::move(right_term));
        left_term = std::move(binary_formula);
        skip_spaces();
    }
    return left_term;
}

std::vector<std::unique_ptr<ares::formula::Formula>>
RuleParser::parse_formula_vector() {
    std::vector<std::unique_ptr<ares::formula::Formula>> result;
    auto new_atom = parse_head_atom();
    if (new_atom != nullptr) {
        result.push_back(std::move(new_atom));
    }
    skip_spaces();
    while (is_next_char_conjunction_operator()) {
        skip_conjunction_operator();
        auto new_atom = parse_head_atom();
        if (new_atom != nullptr) {
            result.push_back(std::move(new_atom));
        }
        skip_spaces();
    }
    return result;
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_binary_operator() {
    std::unique_ptr<ares::formula::Formula> result;
    if (is_next_char('&')) {
        skip_expected_char('&');
        skip_expected_char('&');
        result = std::make_unique<ares::formula::Conjunction>();
    }
    // else {
    // skip_expected_char('|');
    // skip_expected_char('|');
    //// result = new Disjunction();
    //}
    return result;
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_term() {
    std::unique_ptr<ares::formula::Formula> result;
    skip_spaces();
    if (is_next_char('(')) {
        result = parse_complex_formula();
    } else if (is_next_char('[')) {
        result = parse_unary_formula();
    } else {
        result = parse_atom();
    }
    return result;
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_atom() {
    std::unique_ptr<ares::formula::Formula> result;
    skip_spaces();
    if (is_next_char_algebra_operator()) {
        result = parse_algebra_atom();
    } else if (is_next_char_condition_operator()) {
        result = parse_condition_atom();
    } else if (is_next_char_equals_sign()) {
        result = parse_assignment_atom();
    } else {
        result = parse_predicate_atom();
    }
    return result;
}

std::vector<std::string> RuleParser::parse_argument_vector() {
    std::vector<std::string> argument_vector;
    skip_spaces();
    if (is_next_char('(')) {
        skip_next_char();
        skip_spaces();
        if (is_next_char_letter_or_digit()) {
            std::string argument = parse_float_argument();
            argument_vector.push_back(argument);
            skip_spaces();
            while (is_next_char(',')) {
                skip_next_char();
                skip_spaces();
                std::string argument = parse_float_argument();
                argument_vector.push_back(argument);
                skip_spaces();
            }
        }
        skip_expected_char(')');
    }
    return std::move(argument_vector);
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_predicate_atom() {
    auto predicate = parse_identifier();
    auto argument_vector = parse_argument_vector();
    return std::make_unique<ares::formula::Atom>(std::move(predicate),
                                                 std::move(argument_vector));
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_assignment_atom() {
    auto predicate = parse_assignment_operator();
    auto argument_vector = parse_argument_vector();
    if (argument_vector.size() != 2) {
        syntax_error("Not a valid assignment. Expected exactly 2 arguments");
    }
    return std::make_unique<ares::formula::Math>(std::move(predicate),
                                                 std::move(argument_vector));
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_condition_atom() {
    auto predicate = parse_condition_operator();
    auto argument_vector = parse_argument_vector();
    if (argument_vector.size() != 2) {
        syntax_error(
            "Not a valid condition atom. Expected exactly 2 arguments");
    }
    return std::make_unique<ares::formula::Math>(std::move(predicate),
                                                 std::move(argument_vector));
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_algebra_atom() {
    auto predicate = parse_algebra_operator();
    auto argument_vector = parse_argument_vector();
    if (argument_vector.size() != 3) {
        syntax_error("Not a valid algebra atom. Expected exactly 3 arguments");
    }
    return std::make_unique<ares::formula::Math>(std::move(predicate),
                                                 std::move(argument_vector));
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_unary_formula() {
    std::unique_ptr<ares::formula::Formula> result;
    skip_expected_char('[');
    skip_spaces();
    char unary_operator = peek_next_char();
    switch (unary_operator) {
    case 'D':
        result = parse_diamond();
        break;
    case 'B':
        result = parse_box();
        break;
    case '!':
        // result = parse_negation();
        break;
    case '@':
        result = parse_time_reference();
        break;
    case '$':
        result = parse_time_window();
        break;
    case '#':
        result = parse_tuple_window();
        break;
    default:
        syntax_error("Not a valid unary operator");
    }
    return result;
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_diamond() {
    skip_expected_char('D');
    skip_spaces();
    skip_expected_char(']');
    auto child = parse_term();
    return std::make_unique<ares::formula::Diamond>(std::move(child));
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_box() {
    skip_expected_char('B');
    skip_spaces();
    skip_expected_char(']');
    auto child = parse_term();
    return std::make_unique<ares::formula::Box>(std::move(child));
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_negation() {
    // TODO
    return std::make_unique<ares::formula::Atom>("TODO");
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_inertia_operator() {
    std::unique_ptr<ares::formula::Formula> result;
    skip_expected_char('I');
    skip_spaces();
    skip_expected_char(',');
    skip_spaces();
    std::string argument = parse_identifier();
    skip_spaces();
    skip_expected_char(']');
    inertia_variables.insert(argument);
    result = nullptr;
    return result;
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_time_reference() {
    skip_expected_char('@');
    skip_spaces();
    skip_expected_char(',');
    skip_spaces();
    std::string argument = parse_identifier();
    skip_spaces();
    skip_expected_char(']');
    auto child = parse_term();
    return std::make_unique<ares::formula::TimeReference>(argument,
                                                          std::move(child));
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_time_window() {
    skip_expected_char('$');
    skip_spaces();
    skip_expected_char(',');
    skip_spaces();
    std::string argument = parse_natural_number();
    skip_spaces();
    skip_expected_char(']');
    auto child = parse_term();
    uint64_t window_size = std::stoull(argument);
    return std::make_unique<ares::formula::TimeWindow>(window_size,
                                                       std::move(child));
}

std::unique_ptr<ares::formula::Formula> RuleParser::parse_tuple_window() {
    skip_expected_char('#');
    skip_spaces();
    skip_expected_char(',');
    skip_spaces();
    std::string argument = parse_natural_number();
    skip_spaces();
    skip_expected_char(']');
    auto child = parse_term();
    uint64_t window_size = std::stoull(argument);
    return std::make_unique<ares::formula::TupleWindow>(window_size,
                                                        std::move(child));
}

std::vector<std::unique_ptr<ares::rule::Rule>> RuleParser::get_rules() {
    return parse_program();
}

} // namespace ares::rule
