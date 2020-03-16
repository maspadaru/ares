#ifndef ARES_EXAMPLE_EXAMPLE_PARSER_H
#define ARES_EXAMPLE_EXAMPLE_PARSER_H

#include <algorithm>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <util/format_exception.h>
#include <util/grounding.h>
#include <util/timeline.h>

namespace ares {
namespace example {

enum class TokenType {
    OPERATOR,
    OPEN_PARENTHESES,
    CLOSED_PARENTHESES,
    IDENTIFIER
};

struct Token {
    TokenType type;
    std::string value;
};

// Helper functions:

static inline void rtrim(std::string *s);

static inline void ltrim(std::string *s);

static inline void trim(std::string *s);

static inline void syntax_error(std::string error_message);

class ExampleParser {
  private:
    uint64_t current_tuple_counter = 0;

    std::vector<std::shared_ptr<ares::util::Grounding>> argument_stack;

    Token recognize(std::string token_string) const;

    std::vector<Token> tokenize(std::string &rule_string) const;

    void add_token_attempt(std::vector<Token> &token_vector,
                           std::ostringstream &token_stream) const;

    void add_new_token(std::vector<Token> &token_vector, TokenType type,
                       char value_char) const;

    std::vector<std::shared_ptr<ares::util::Grounding>>
    parse_token_vector(ares::util::Timeline const &timeline,
                       std::vector<Token> const &input_token_vector);

  public:
    ~ExampleParser() = default;

    std::vector<std::shared_ptr<ares::util::Grounding>>
    parse_data(ares::util::Timeline &timeline,
               std::vector<std::string> &raw_data_vector);
};

} // namespace example
} // namespace ares

#endif // ARES_EXAMPLE_EXAMPLE_PARSER_H
