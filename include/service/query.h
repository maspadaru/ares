#ifndef ARES_SERVICE_QUERY_H
#define ARES_SERVICE_QUERY_H

#include <set>
#include <string>

#include "formula/formula.h"
#include "rule/shared.h"
#include "service/request.h"
#include "service/request_type.h"
#include "util/grounding.h"

namespace ares {
namespace service {

class Query : public Request {
  private:
    RequestType type = RequestType::QUERY;
    std::unique_ptr<formula::Formula> query_formula;
    size_t formula_arity = 0;
    std::string query_predicate;
    util::Grounding trigger;
    std::vector<size_t> known_index_vector;
    std::vector<std::shared_ptr<util::Grounding>> const &database_facts;
    uint64_t request_time;

    // void
    // init(formula::Formula *query_formula, util::Grounding trigger,
    // std::vector<std::string> known_variables,
    // std::vector<std::shared_ptr<util::Grounding>> const &database_facts);

    bool is_known_variable_match(
        std::shared_ptr<util::Grounding> const &db_fact) const;

    std::set<std::shared_ptr<util::Grounding>,
             util::GroundingPredicateSubstitutionCompare>
    compute_answer();

  public:
    explicit Query(
        std::unique_ptr<formula::Formula> query_formula,
        util::Grounding trigger, std::vector<size_t> known_index_vector,
        std::vector<std::shared_ptr<util::Grounding>> const &database_facts);

    Query(Query const &other);
    Query(Query &&other) noexcept;

    ~Query() override = default;

    std::string evaluate() override;
};

} // namespace service
} // namespace ares

#endif // ARES_SERVICE_QUERY_H
