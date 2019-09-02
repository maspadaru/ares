#ifndef LASER_FORMULA_EXTENDED_TIME_WINDOW_H
#define LASER_FORMULA_EXTENDED_TIME_WINDOW_H

#include <string>

#include "formula/formula.h"
#include "util/grounding.h"

namespace laser::formula {

/**
 * Time Window Formula
 */
class TimeWindow : public Formula {
  private:
    uint64_t past_size = 0;   // L
    uint64_t future_size = 0; // U
    uint64_t step_size = 0;   // D
    uint64_t pivot_time = 0;
    Formula *child;

    util::Timeline alter_timeline(util::Timeline timeline) const;
    uint64_t compute_horizon_time(uint64_t grounding_consideration_time,
                                  uint64_t grounding_horizon_time,
                                  uint64_t current_time) const;

  public:

    TimeWindow() = default;
    ~TimeWindow() override;

    Formula &create() const override;
    Formula &clone() const override;

    Formula &move() override;

    void set_head(bool is_head) override;

    bool is_head() const override;

    FormulaType get_type() const override;

    std::vector<std::string> const &get_predicate_vector() const override;

    std::map<std::string, size_t> const &get_arity_map() const override;

    std::vector<std::string> const &get_variable_names() const override;

    int get_variable_index(std::string const &variable_name) const override;

    size_t get_number_of_variables() const override;

    bool evaluate(
        util::Timeline const &timeline, size_t previous_step,
        std::vector<std::shared_ptr<util::Grounding>> const &facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_timepoint(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_step(util::Timeline const &timeline) override;

    void add_child(formula::Formula *child) override;

    explicit TimeWindow(uint64_t size, Formula *child);

    explicit TimeWindow(uint64_t past_size, uint64_t future_size,
                        uint64_t step_size, Formula *child);

    std::vector<formula::Formula *> get_children() const override;

    uint64_t get_window_size() const override;
};

} // namespace laser::formula

#endif // LASER_FORMULA_EXTENDED_TIME_WINDOW_H
