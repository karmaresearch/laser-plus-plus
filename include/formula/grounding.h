//
// Created by mike on 6/17/18.
//


#ifndef LASER_FORMULA_GROUNDING_H
#define LASER_FORMULA_GROUNDING_H

#include <climits>
#include <string>
#include <unordered_map>

namespace laser {
namespace formula {

/**
 * Annotated Grounding (_alpha_-_sigma_)
 * (Definition 7)
 * Maps each variable in the formula _alpha_ to a constant value due to
 * substitution _sigma_
 * The grounding is annotated with consideration and horizon values
 */
class Grounding {

private:
    unsigned long long int consideration_time = 0;
    unsigned long long int horizon_time = ULLONG_MAX;
    unsigned long long int consideration_count = 0;
    unsigned long long int horizon_count = ULLONG_MAX;
    bool is_background_fact_m = false;
    // key: variable index; value: constant substitution_map of key in this grounding
    std::unordered_map<int, std::string> substitution_map;
public:
// constructors & destructors

    Grounding(unsigned long long int consideration_time, unsigned long long int horizon_time,
              unsigned long long int consideration_count, unsigned long long int horizon_count);

    explicit Grounding(bool is_background_fact_m);

// getters & setters

    unsigned long long int get_consideration_time() const;

    unsigned long long int get_horizon_time() const;

    unsigned long long int get_consideration_count() const;

    unsigned long long int get_horizon_count() const;

    void set_horizon_time(unsigned long long int horizon_time);

// const methods

    std::string get_substitution(int variable_index) const;

// methods

    void add_substitution(int variable_index, std::string constant);

};


} // namespace formula
} // namespace laser


#endif // LASER_FORMULA_GROUNDING_H
