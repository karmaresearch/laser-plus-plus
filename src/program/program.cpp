//
// Created by mike on 6/15/18.
//

#include <utility>
#include "program/program.h"


namespace laser {
namespace program {

// constructors & destructors

Program::~Program() {
    rule_vector.clear();
}

void Program::init_rules(std::vector<rule::Rule> rules) {
    for (const auto &rule : rules) {
        auto local_rule = new rule::Rule(rule);
        rule_vector.push_back(local_rule);
    }
}


Program::Program(
        input::InputManager input_manager,
        output::OutputManager output_manager) : input_manager(
        input_manager), output_manager(output_manager) {
    auto rules = input_manager.get_rules();
    init_rules(std::move(rules));
    strata.stratify(rule_vector);

    if (input_manager.is_initialised_background_reader()) {
        std::unordered_map<std::string, std::vector<formula::Formula *>>
                background_facts;
        std::tie(number_of_background_facts, background_facts)
                = input_manager.get_background_facts();
        if (number_of_background_facts > 0) {
            strata.evaluate(current_time, current_tuple_counter,
                    background_facts);
        }
    }

    if (input_manager.fetch_stream_metadata()) {
        stream_start_time = input_manager.get_stream_start_time();
        stream_end_time = input_manager.get_stream_end_time();
        has_timeline = true;
    }

//    for(auto rule : rule_vector) {
//        rule->debug_print();
//    }
}

// getters & setters

unsigned long long int Program::get_current_time() const {
    return current_time;
}

unsigned long long int Program::get_current_tuple_counter() const {
    return current_tuple_counter;
}

int Program::get_number_of_new_conclusions() const {
    return number_of_new_conclusions;
}

// methods

bool Program::is_done() {
    bool result = false;
    result = has_timeline && current_time >= stream_end_time;
    return result;
}

void Program::expire_outdated_groundings(
        unsigned long long int current_time,
        unsigned long long int current_tuple_counter) {
    for (auto rule :rule_vector) {
        rule->expire_outdated_groundings(current_time, current_tuple_counter);
    }
}

bool Program::eval(unsigned long long int request_time_point) {
    bool has_derived_new_conclusions = false;
    std::unordered_map<std::string, std::vector<formula::Formula *>>
            stream_facts;
    std::cerr << "current_time: " << current_time << std::endl;
    std::tie(current_time, current_tuple_counter, stream_facts)
            = input_manager.get_stream_facts(request_time_point);
    expire_outdated_groundings(current_time, current_tuple_counter);
    has_derived_new_conclusions = strata.evaluate(current_time,
            current_tuple_counter,
            stream_facts);
    return has_derived_new_conclusions;
}

std::unordered_map<std::string, std::vector<formula::Formula *>>
Program::get_new_conclusions() {
    // TODO Get new conclusions from each rule as facts of type Formula*
    // TODO combine them in a map
    return std::unordered_map<std::string, std::vector<formula::Formula *>>();
}


void Program::write_output(
        std::unordered_map<std::string, std::vector<formula::Formula *>>
        new_conclusions) {
    output_manager.write_output(std::move(new_conclusions));
}

bool Program::evaluate() {
    unsigned long long int request_time_point = current_time + 1;
    bool has_derived_new_conclusions = eval(request_time_point);
    if (has_derived_new_conclusions) {
        auto new_conclusions = get_new_conclusions();
        write_output(new_conclusions);
    }
    return has_derived_new_conclusions;
}hh

} // namespace program
} // namespace laser
