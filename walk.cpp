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
#include <omp.h>

/*
bool continue_walking(double alpha) {
    std::random_device rd;
    std::mt19937 engine(rd());
    std::bernoulli_distribution dist(alpha);
    return dist(engine);
}
*/

void print_top_k_scores(const vector<double>& scores, int k) {
    cout << "\nTop " << k << " scores:\n";
    assert(k <= scores.size());
    
    vector<pair<int, double>> score_pairs;
    score_pairs.reserve(scores.size());
    
    for (int i = 0; i < scores.size(); i++) {
        score_pairs.emplace_back(make_pair(i, scores.at(i)));
    }
    
    auto comp = [](pair<int, double> left, pair<int, double> right){ return left.second > right.second; };
    std::sort(score_pairs.begin(), score_pairs.end(), comp);
    
    for (int j = 0; j < k; j++) {
        cout << score_pairs.at(j).first << " : " << score_pairs.at(j).second << "\n";
    }
    
    cout << "\n";
}



score simulate_walk(const graph& G, const transition_rule& T, const node_id seed,
                    const int history_size, const double alpha, const int n_runs,
                    bool global) {
    
    assert(0 < alpha && alpha < 1);
    assert(history_size >= 0 && n_runs >= 0);
    assert(history_size < G.num_nodes());
    
    score walk_scores(G.num_nodes());
    
    std::random_device rd;
    std::mt19937 engine(rd());
    std::bernoulli_distribution continue_walk(alpha);
    
    if (global) {
        for (node_id i = 0; i < G.num_nodes(); ++i) { // start from each node
            
            for (int r = 0; r < n_runs; ++r) { // do 'n_runs' runs from each node
                state current_state(history_size, i); // start the walk from node i
                
                walk_scores.CP_scores.at(i) += (1-alpha) / (n_runs * G.num_nodes());
                
                while (continue_walk(engine)) { // while we don't terminate the walk
                    T.operator()(G, current_state, engine);
                    walk_scores.CP_scores.at(current_state.get_current_node()) += (1-alpha) / (n_runs * G.num_nodes());
                }
                
                walk_scores.EP_scores.at(current_state.get_current_node()) += 1.0 / (n_runs * G.num_nodes());
            }
        }
    }
    
    else {
        for (int r = 0; r < n_runs; ++r) {
            state current_state(history_size, seed);
            
            walk_scores.CP_scores.at(seed)++;
            
            while (continue_walk(engine)) {
                T.operator()(G, current_state, engine);
                walk_scores.CP_scores.at(current_state.get_current_node())++;
            }
            
            walk_scores.EP_scores.at(current_state.get_current_node())++;
        }

        for (int i = 0; i < G.num_nodes(); ++i) {
            walk_scores.CP_scores[i] *= (1.0-alpha)/n_runs;
            walk_scores.EP_scores[i] *= 1.0/n_runs;
        }
    }
    
    return walk_scores;
}



bool temporal_path_exists(const graph& G, const node_id source, const node_id target, timestamp t_prev, int num_steps) {
    // we start from the source node. if we hit the target node we increment the count and leave the queue

    // assert(G.has_timestamps());
    // assert(G.node_exists(source));
    // assert(G.node_exists(target));

    if (num_steps >= 5) {
        return false;
    }


    // loop through the temporal neighbors
    const vector<out_edge>& neighbors = G.get_out_edges(source);
    auto comp = [=](const out_edge& e) { return e.t > t_prev; };
    auto first_causal = find_if(neighbors.begin(), neighbors.end(), comp);
    size_t distance = std::distance(first_causal, neighbors.end());

    if (distance == 0) {
        return false;
    }

    if (source == 603) {
        cout << distance << '\n';
    }
    
    for (auto it = first_causal; it != neighbors.end(); it++) {
        if (it->n == target) {
            return true;
        }
    }


    for (auto it = first_causal; it != neighbors.end(); it++) {
        if (temporal_path_exists(G, it->n, target, it->t, num_steps+1)) {
            return true;
        }
    }

    return false;
}
