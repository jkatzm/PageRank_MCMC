//
//  transition_rule.hpp
//  C++HOP_expander
//
//  Created by Katz, Jordan Alexander on 2/4/19.
//  Copyright © 2019 Katz, Jordan Alexander. All rights reserved.
//

#ifndef transition_rule_hpp
#define transition_rule_hpp

#include "graph.hpp"
#include "state.hpp"
#include <random>

using namespace std;

int randint(size_t upper_bound, std::mt19937& engine); // returns a random int in the half-open interval [0, upper_bound)

template <typename T>
const T& get_random_item(const vector<T>& vec, std::mt19937& engine); // returns a random item from the vector




struct transition_rule {
    transition_rule(string rule) : rule(rule) {}
    
    void operator()(const graph& G, state& current_state, std::mt19937& engine) const;
    void T_PageRank(const graph& G, state& current_state, std::mt19937& engine) const;
    void T_temporally_causal(const graph& G, state& current_state, std::mt19937& engine) const;
    void T_self_avoiding(const graph& G, state& current_state, std::mt19937& engine) const;
    void T_temporal_self_avoiding(const graph& G, state& current_state, std::mt19937& engine) const;
    
private:
    const string rule;
};




#endif /* transition_rule_hpp */
