//
//  walk.cpp
//  C++HOP_expander
//
//  Created by Katz, Jordan Alexander on 2/4/19.
//  Copyright © 2019 Katz, Jordan Alexander. All rights reserved.
//

#include "walk.hpp"
#include <random>
#include <cassert>

bool continue_walking(double alpha) {    
    std::random_device rd;
    std::mt19937 engine(rd());
    std::bernoulli_distribution dist(alpha);
    return dist(engine);
}

score simulate_walk(const graph& G, const transition_rule& T, const node_id seed,
                     const int history_size, const double alpha, const int n_runs) {
    
    assert(0 < alpha && alpha < 1);
    assert(history_size >= 0 && n_runs >= 0);
    assert(history_size < G.num_nodes());
    
    score S(G.num_nodes());

    for (int r = 0; r < n_runs; ++r) {
        state current_state(history_size, seed);
        
        S.CP_scores.at(seed) += (1-alpha) / n_runs;
        
        while (continue_walking(alpha)) {
            T.operator()(G, current_state);
            S.CP_scores.at(current_state.get_current_node()) += (1-alpha) / n_runs;
        }
        
        S.EP_scores.at(current_state.get_current_node()) += 1.0 / n_runs;
    }
    
    return S;
}


