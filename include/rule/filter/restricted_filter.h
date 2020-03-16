#ifndef ARES_RULE_FILTER_RESTRICTED_FILTER_H
#define ARES_RULE_FILTER_RESTRICTED_FILTER_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "formula/extended/conjunction.h"
#include "formula/formula.h"
#include "formula/formula_type.h"
#include "rule/chase_filter.h"
#include "rule/shared.h"
#include "util/database.h"
#include "util/global.h"
#include "util/grounding.h"
#include "util/settings.h"
#include "util/shared.h"
#include "util/timeline.h"

namespace ares::rule {

/**
 * Restricted Chase Filter
 */
class RestrictedFilter : public ChaseFilter {
  private:
    std::string const BOUND_VALUE_PLACEHOLDER = "";
    uint64_t null_value_count = 0;
    size_t head_variables_count = 0;
    size_t head_atom_count = 0;
    uint64_t current_timepoint;
    bool has_event_variables = false;
    bool use_global_nulls = true;
    bool has_database_facts = false;
    std::vector<formula::Formula *> head_atoms;
    std::vector<bool> is_event_variable;
    std::vector<std::string> head_variables;
    std::vector<std::string> free_variables;
    std::vector<std::shared_ptr<util::Grounding>> current_step_facts;
    // position of event variables in head_variables
    std::vector<int> event_variable_list;
    // position of free variables in head_variables
    std::vector<int> free_variable_list;
    // index = variable position in head_variable vector
    // value = variable position in input_fact (free_variables vector)
    std::vector<int> body_head_var_index;
    std::vector<std::string> head_atom_predicates;
    std::vector<std::vector<std::shared_ptr<util::Grounding>> *>
        database_facts; // positions correspond to head_atoms
    std::vector<std::vector<std::shared_ptr<util::Grounding>> *>
        inertia_facts; // positions correspond to head_atoms
    std::vector<std::vector<std::shared_ptr<util::Grounding>>> head_facts;
    std::vector<std::vector<std::string>> current_step_substitutions;
    std::vector<std::vector<std::string>> event_substitutions;
    std::vector<std::vector<std::string>> current_timepoint_substitutions;
    std::vector<std::vector<std::string>> inertia_substitutions;
    // index = head_atom,
    // subvector index = variable index in atom
    // subvector value = variable index in head_variables; -1 if absent
    std::vector<std::vector<int>> head_atoms_var_index_atom_to_head;
    std::unordered_map<std::string, int> free_variable_index;
    std::unordered_map<std::string, int> bound_variable_index;
    std::unordered_map<std::string, int> head_variable_index;

    // moethods

    void update_head_facts(util::Timeline const &timeline);
    void init_head_facts();
    void init_head_atoms(
        std::vector<std::unique_ptr<formula::Formula>> const &head_atoms);
    void init_event_variable_list();
    void init_free_variable_list();
    void init_body_head_var_index();
    void init_head_atom_predicates(
        std::vector<std::unique_ptr<formula::Formula>> const &head_atoms);
    void init_head_atoms_var_index_atom_to_head(
        std::vector<std::unique_ptr<formula::Formula>> const &head_atoms);
    void init_database_facts(util::Database &database);
    std::string generate_new_value(std::string const &var_name);
    std::shared_ptr<util::Grounding>
    generate_fact(std::vector<std::string> const &substitution,
                  std::shared_ptr<util::Grounding> const &input_fact) const;
    std::vector<std::string> extract_substitution(
        std::shared_ptr<util::Grounding> const &input_fact) const;
    std::vector<std::string> extract_event_substitution(
        std::vector<std::string> const &substitution) const;
    bool is_substitution_match(std::vector<std::string> const &left,
                               std::vector<std::string> const &right) const;
    bool has_match_current_timepoint(
        std::vector<std::string> const &initial_substitution) const;
    bool is_full_match(std::vector<std::string> const &substitution) const;
    bool is_event_match(std::vector<std::string> const &substitution) const;
    bool is_fact_match(std::vector<std::string> const &substitution,
                       std::shared_ptr<util::Grounding> const &fact,
                       size_t head_atom_index) const;
    std::vector<std::string>
    fact_extend(std::vector<std::string> const &substitution,
                std::shared_ptr<util::Grounding> const &fact,
                size_t head_atom_index) const;
    //std::vector<std::string>
    //event_extend(std::vector<std::string> const &left,
                 //std::vector<std::string> const &right) const;
    std::vector<std::string>
    generate_extension(std::vector<std::string> const &substitution);
    void search_database(std::shared_ptr<util::Grounding> const &input_fact);
    //std::vector<std::vector<std::string>>
    //match_events(std::vector<std::string> const &initial_substitution) const;
    std::vector<std::vector<std::string>>
    event_match_list(std::vector<std::string> const &initial_substitution,
               std::vector<std::shared_ptr<util::Grounding>> const &fact_vector,
               size_t head_atom_index) const;
    std::vector<std::vector<std::string>>
    match_list(std::vector<std::string> const &initial_substitution,
               std::vector<std::shared_ptr<util::Grounding>> const &fact_vector,
               size_t head_atom_index) const;
    std::vector<std::vector<std::string>>
    get_all_database_match(std::vector<std::string> const &initial_substitution,
                   size_t head_atom_index) const;
    std::vector<std::vector<std::string>>
    get_all_event_match(std::vector<std::string> const &initial_substitution,
                   size_t head_atom_index) const;
    bool
    find_database_match(std::shared_ptr<util::Grounding> const &input_fact,
                        std::vector<std::string> const &initial_substitution,
                        size_t head_atom_index);

    bool
    find_event_match(std::shared_ptr<util::Grounding> const &input_fact,
                        std::vector<std::string> const &initial_substitution,
                        size_t head_atom_index);

  public:
    RestrictedFilter() = default;

    void init(std::vector<std::unique_ptr<formula::Formula>> const &head_atoms,
              std::vector<std::string> const &head_variables,
              std::vector<std::string> const &free_variables,
              std::vector<std::string> const &bound_variables,
              std::vector<bool> const &is_event_variable,
              std::vector<std::string> const &frontier_variables,
              bool has_event_variables) override;

    void update(util::Timeline const &timeline, size_t previous_step,
                util::Database &database) override;

    std::vector<std::shared_ptr<util::Grounding>>
    build_chase_facts(util::Timeline const &timeline, size_t previous_step,
                      std::vector<std::shared_ptr<util::Grounding>> const
                          &input_facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;
};

} // namespace ares::rule

#endif // ARES_RULE_FILTER_RESTRICTED_FILTER_H
