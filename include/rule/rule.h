//
// Created by mike on 6/15/18.
//
#ifndef LASER_RULE_RULE_H
#define LASER_RULE_RULE_H

#include <iostream>

#include "formula/formula.h"
#include "util/timeline.h"

namespace laser {
namespace rule {

class Rule {
  private:
    formula::Formula &head;
    formula::Formula &body;

    /**
     * Maps variables positions in the head's Grounding Table to positions in 
     * the body's Grounding Table.
     * E.g.: p(X, Y) :- q(Y, Z, X) -> { [0, 2], [1, 0] }
     */  
    std::unordered_map<size_t, size_t> variable_map;

    formula::Grounding
    convert_to_head_grounding(formula::Grounding const &grounding) const;

    void compute_variable_map();
    
    void init();

  public:
    // constructors & destructors

    explicit Rule(formula::Formula *head_formula,
                  formula::Formula *body_formula);

    Rule(Rule const &other);     // copy constructor
    Rule(Rule &&other) noexcept; // move constructor

    ~Rule();

    Rule &operator=(Rule const &other);     // copy assignment
    Rule &operator=(Rule &&other) noexcept; // move assignment

    // getters & setters

    formula::Formula &get_head() const;

    // methods

    /**
     * Derives new conclusions based on the head of the rule and all groundings
     * that satisfy the body.
     * Also clears previous derivations that no longer hold.
     * @return True if new conclusions have been derived at the current
     * time point and tuple counter
     */
    bool evaluate(
        util::Timeline timeline,
        std::unordered_map<std::string, std::vector<formula::Grounding>> facts);

    void expire_outdated_groundings(util::Timeline timeline);

    bool derive_conclusions(util::Timeline timeline);

    void debug_print() const;

    template <typename T>
    void debug_print(std::string const &message, T const &value) const;
};

} // namespace rule
} // namespace laser

#endif // LASER_RULE_RULE_H
