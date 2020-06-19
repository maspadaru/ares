#ifndef ARES_CORE_REASONER_H
#define ARES_CORE_REASONER_H

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <unordered_map>
#include <vector>

#include "core/io_manager.h"
#include "core/program.h"
#include "rule/rule.h"
//#include "service/service_manager.h"
#include "util/chase_algorithm.h"
#include "util/grounding.h"
#include "util/settings.h"
#include "util/timeline.h"

namespace ares::core {

class Reasoner {
  private:
    uint64_t total_facts_read = 0;
    uint64_t total_conclusions = 0;
    std::mutex fact_map_mutex;
    std::mutex conclusion_map_mutex;
    std::mutex output_ready_mutex;
    std::condition_variable output_ready_condition;
    IOManager *io_manager;
    // service::ServiceManager *service_manager;
    std::vector<std::unique_ptr<ares::rule::Rule>> &rule_vector;
    bool is_listen_on = false;
    std::unordered_map<uint64_t, std::vector<std::shared_ptr<util::Grounding>>>
        fact_map;
    std::vector<std::shared_ptr<util::Grounding>> background_fact_vector;
    std::unordered_map<uint64_t, std::vector<std::shared_ptr<util::Grounding>>>
        conclusion_map;
    std::vector<double> runtimes;

    void read(util::Timeline timeline);
    void evaluate(util::Timeline timeline);
    void write(util::Timeline timeline);
    void listen(util::Timeline timeline);

    void insert_facts(uint64_t timepoint,
                      std::vector<std::shared_ptr<util::Grounding>> facts);

    std::vector<std::shared_ptr<util::Grounding>> get_facts(uint64_t timepoint);

    void insert_conclusions(
        uint64_t timepoint,
        std::vector<std::shared_ptr<util::Grounding>> conclusions);

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions(uint64_t timepoint);

  public:
    explicit Reasoner(
        std::vector<std::unique_ptr<ares::rule::Rule>> &rule_vector,
        IOManager *io_manager);

    // explicit Reasoner(std::vector<rule::Rule> *rule_vector,
    // IOManager *io_manager,
    // service::ServiceManager *service_manager);

    void start();

    uint64_t get_total_facts_read() const;
    uint64_t get_total_conclusions() const;
    std::vector<double> const &get_runtimes() const;
};

} // namespace ares::core

#endif // ARES_CORE_REASONER_H
