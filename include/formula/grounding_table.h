//
// Created by mike on 6/16/18.
//
#ifndef LASER_FORMULA_GROUNDING_TABLE_H
#define LASER_FORMULA_GROUNDING_TABLE_H

#include <list>
#include <set>
#include <string>
#include <unordered_map>
#include <iostream>

#include "formula/grounding.h"

namespace laser {
namespace formula {

class GroundingTable {
  private:
    // grounding_map: key: horizon_time
    //        value: Set of all groundings sharing consideration_time
    std::unordered_map<uint64_t, std::set<Grounding>> grounding_map;
    std::set<Grounding> recent_groundings_set;
    std::vector<std::string> variable_names;
    std::unordered_map<std::string, int> variable_index;
    size_t size = 0;

    template <typename T>
    void debug_print(std::string const &message, T const &value) const;
  public:

    size_t get_size() const;


    std::vector<std::string> get_variable_names() const;

    void set_variable_names(std::vector<std::string> variable_names);

    /** Returns the contents of recent groundings set as a vector, and clears it
     */
    std::vector<Grounding> get_recent_groundings();

    bool has_recent_groundings();

    void add_variable_name(std::string const &variable_name);

    size_t get_number_of_variables() const;

    int get_variable_index(std::string const& variable_name) const;

    std::vector<Grounding> get_all_groundings();

    void add_grounding(Grounding const &grounding);

    void add_grounding_vector(std::vector<Grounding> const &grounding_vector);

    /**
     * Removes all annotated grounding that have expired due to horizon time
     * or tupple counter
     */
    void expire_outdated_groundings(uint64_t expiration_time,
                                    uint64_t expiration_tuple_counter);

};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_GROUNDING_TABLE_H
