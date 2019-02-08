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

using namespace std;

int randint(size_t upper_bound);
// returns a random int in the half-open interval [0, upper_bound)

template <typename T>
const T& get_random_item(const vector<T>& vec);
// returns a random item from the vector


struct transition_rule {
    transition_rule(string rule) : rule(rule) {}
    void operator()(const graph& G, state& current_state) const;
    
private:
    const string rule;
};


void T_PageRank(const graph& G, state& current_state);
void T_temporally_causal(const graph& G, state& current_state);
void T_self_avoiding(const graph& G, state& current_state);

#endif /* transition_rule_hpp */
