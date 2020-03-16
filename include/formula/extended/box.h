#ifndef ARES_FORMULA_EXTENDED_BOX_H
#define ARES_FORMULA_EXTENDED_BOX_H

#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "formula/formula.h"
#include "formula/formula_type.h"
#include "formula/grounding_table.h"
#include "formula/math_operator.h"
#include "util/grounding.h"

namespace ares::formula {

/**
 * Box Formula
 */
class Box : public Formula {
  private:
    bool is_head_m = false;

    std::unique_ptr<Formula> child;

    GroundingTable grounding_table;

    std::map<std::string, std::shared_ptr<util::Grounding>> box_map;

    std::pair<bool, std::shared_ptr<util::Grounding>>
    adjust_annotation(std::shared_ptr<util::Grounding> const &box_grounding,
                      std::shared_ptr<util::Grounding> const &child_grounding,
                      util::Timeline const &timeline) const;

    std::vector<std::shared_ptr<util::Grounding>>
    compute_box_conclusions(util::Timeline const &timeline);

    void
    update_box_map(std::vector<std::shared_ptr<util::Grounding>> const &facts,
                   util::Timeline const &timeline);

  public:
    Box() = default;
    explicit Box(std::unique_ptr<formula::Formula> child);

    std::unique_ptr<formula::Formula> clone() const override;

    void set_head(bool is_head) override;

    bool is_head() const override;

    FormulaType get_type() const override;

    std::vector<std::string> get_math_arguments() const override;

    MathOperator get_math_operator() const override;

    std::vector<std::string> const &get_predicate_vector() const override;

    std::map<std::string, size_t> const &get_arity_map() const override;

    std::vector<std::string> const &get_variable_names() const override;

    int get_variable_index(std::string const &variable_name) const override;

    size_t get_number_of_variables() const override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_timepoint(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_step(util::Timeline const &timeline) override;

    bool evaluate(
        util::Timeline const &timeline, size_t previous_step,
        std::vector<std::shared_ptr<util::Grounding>> const &facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;

    void add_child(std::unique_ptr<formula::Formula> child) override;

    std::vector<std::unique_ptr<formula::Formula> const *>
    get_children() const override;

    uint64_t get_window_size() const override;
};

} // namespace ares::formula

#endif // ARES_FORMULA_EXTENDED_BOX_H
