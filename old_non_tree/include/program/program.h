//
// Created by mike on 6/15/18.
//
#ifndef LASER_PROGRAM_PROGRAM_H
#define LASER_PROGRAM_PROGRAM_H

#include <vector>
#include <string>
#include <unordered_map>

#include "excetion/request_exception.h"
#include "input/input_manager.h"
#include "output/output_manager.h"
#include "formula/formula.h"
#include "rule/rule.h"
#include "strata.h"

namespace laser {
namespace program {

class Program {
private:

    input::InputManager input_manager;
    output::OutputManager output_manager;

    uint64_t current_time = 0;
    uint64_t current_tuple_counter = 0;
    uint64_t stream_start_time = 0;
    uint64_t stream_end_time = 0;
    int number_of_new_conclusions = 0;
    bool has_timeline = false;

    uint64_t number_of_background_facts = 0;

    Strata strata;
    std::vector<rule::Rule *> rule_vector;

// methods

    bool eval(uint64_t request_time_point);

    void expire_outdated_groundings(
            uint64_t current_time,
            uint64_t current_tuple_counter);

    std::unordered_map<std::string, std::vector<formula::Formula *>>
    get_new_conclusions();

    void write_output(
            std::unordered_map<std::string, std::vector<formula::Formula *>>
            new_conclusions);

    void init_rules(std::vector<rule::Rule> rules);

public:

// constructors & destructors

    Program(
            input::InputManager input_manager,
            output::OutputManager output_manager);

    virtual ~Program();

// getters & setters

    uint64_t get_current_time() const;

    uint64_t get_current_tuple_counter() const;

    int get_number_of_new_conclusions() const;

// methods

    /**
     *
     * @return True if new conclusions were derived from the input facts in
     * the current program time point
     */
    bool evaluate();

    bool is_done();

};

} // namespace program
} // namespace laser



#endif // LASER_PROGRAM_PROGRAM_H
