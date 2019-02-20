//
//  walk.hpp
//  C++HOP_expander
//
//  Created by Katz, Jordan Alexander on 2/4/19.
//  Copyright © 2019 Katz, Jordan Alexander. All rights reserved.
//

#ifndef walk_hpp
#define walk_hpp

#include "transition_rule.hpp"

/*
bool continue_walking(double alpha);
// returns true with Pr[alpha], false with Pr[1-alpha]
*/

struct score {
    score(size_t n) {
        EP_scores = vector<double>(n, 0.0);
        CP_scores = vector<double>(n, 0.0);
        assert(EP_scores.size() == n && CP_scores.size() == n);
    }
    
    vector<double> EP_scores; // MC end point scores
    vector<double> CP_scores; // MC complete path scores
};

void print_top_k_scores(const vector<double>& scores, int k);

score simulate_walk(const graph& G, const transition_rule& T, const node_id seed,
                    const int history_size, const double alpha, const int n_runs,
                    bool global);

                    // "alpha" is the probability the walker continues to walk (as opposed to terminate). must be between 0 and 1
                    // "T" tells the walker how to move based on the current state


bool temporal_path_exists(const graph& G, const node_id source, const node_id target, timestamp t_prev, int num_steps);

#endif /* walk_hpp */
