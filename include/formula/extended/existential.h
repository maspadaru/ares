#ifndef LASER_FORMULA_EXTENDED_EXISTENTIAL_H
#define LASER_FORMULA_EXTENDED_EXISTENTIAL_H

#include <string>

#include "formula/formula.h"
#include "util/grounding.h"

namespace laser {
namespace formula {

/**
 * Formula begining with a Existential Quantifier operator
 */
class Existential : public Formula {
  private:
    Formula *child;
    std::vector<std::string> bound_variables;
    std::vector<std::string> free_variables;
    std::vector<std::string> child_variables;
    std::unordered_map<std::string, int> free_variable_index;
    std::unordered_map<std::string, int> bound_variable_index;
    std::unordered_map<size_t, std::vector<std::string>> skolem_map;
    uint64_t null_value_count = 0;

    std::shared_ptr<util::Grounding>
    make_skolem(std::shared_ptr<util::Grounding> const &body_grounding);

    std::string generate_new_value(std::string const &var_name);

    void init_variable_vectors();

    std::unordered_map<std::string, int>
    make_index(std::vector<std::string> const &vector);

  public:
    explicit Existential(std::vector<std::string> argument_vector,
                         Formula *child);
    Existential() = default;
    ~Existential() override;

    Formula &create() const override;
    Formula &clone() const override;

    Formula &move() override;

    void set_head(bool is_head) override;

    bool is_head() const override;

    FormulaType get_type() const override;

    std::vector<std::string> get_predicate_vector() const override;

    std::vector<std::string> const &get_variable_names() const override;

    int get_variable_index(std::string const &variable_name) const override;

    size_t get_number_of_variables() const override;

    bool evaluate(
        util::Timeline const &timeline,
        std::vector<std::shared_ptr<util::Grounding>> const &facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_timepoint(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_step(util::Timeline const &timeline) override;

    void add_child(formula::Formula *child) override;
};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_EXISTENTIAL_H
