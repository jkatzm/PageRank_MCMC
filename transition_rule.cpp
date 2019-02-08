//
//  transition_rule.cpp
//  C++HOP_expander
//
//  Created by Katz, Jordan Alexander on 2/4/19.
//  Copyright © 2019 Katz, Jordan Alexander. All rights reserved.
//

#include "transition_rule.hpp"
#include <random>
#include <cassert>

int randint(size_t upper_bound) {
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> dist(0, int(upper_bound)-1);
    return dist(engine);
}

template <typename T>
const T& get_random_item(const vector<T>& vec) {
    return vec.at(randint(vec.size()));
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

void transition_rule::operator()(const graph& G, state& current_state) const {
    if (this->rule == "PageRank") {
        assert(G.has_timestamps() == false);
        T_PageRank(G, current_state);
    }
    else if (this->rule == "temporally_causal") {
        assert(G.has_timestamps());
        T_temporally_causal(G, current_state);
    }
    else if (this->rule == "self_avoiding") {
        assert(G.has_timestamps() == false);
        T_self_avoiding(G, current_state);
    }
    else throw "unknown rule";
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

void T_PageRank(const graph& G, state& current_state) {
    node_id next_node;
    const vector<node_id>& neighbors = G.get_out_neighbors(current_state.get_current_node());
    
    if (neighbors.empty()) {
        next_node = randint(G.num_nodes());
    }
    
    else {
        next_node = get_random_item(neighbors);
    }

    current_state.update_history(next_node);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

void T_temporally_causal(const graph& G, state& current_state) {
    node_id next_node;
    const vector<out_edge>& neighbors = G.get_out_edges(current_state.get_current_node());
    
    // get the neighbors whose edges have a timestamp >= prev timestamp
    vector<out_edge> causal_neighbors;
    causal_neighbors.reserve(neighbors.size());
    
    for (const auto& it : neighbors) {
        if (it.t >= current_state.get_previous_timestamp()) {
            causal_neighbors.push_back(it);
        }
    }
    
    // pick a random causal neighbor to jump to
    // if there are none, pick a random node and keep the previous timestamp the same
    if (causal_neighbors.empty()) {
        next_node = randint(G.num_nodes());

    }
    
    else {
        const out_edge& edge = get_random_item(causal_neighbors);
        current_state.update_timestamp(edge.t);
        next_node = edge.n;
    }
    
    current_state.update_history(next_node);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

void T_self_avoiding(const graph& G, state& current_state) {
    node_id next_node;
    const vector<node_id>& neighbors = G.get_out_neighbors(current_state.get_current_node());
    
    // get the neighbors who aren't in the current history
    vector<node_id> nbtw_neighbors;
    nbtw_neighbors.reserve(neighbors.size());
    
    for (auto it : neighbors) {
        if (current_state.does_not_contain(it)) {
            nbtw_neighbors.push_back(it);
        }
    }
    
    // pick a random nbtw neighbor to jump to
    // if there are none, pick a node not in the current history
    if (nbtw_neighbors.empty()) {
        int count = 0;
        
        next_node = randint(G.num_nodes());
        
        while (current_state.contains(next_node)) { // rejection sampling
            count++;
            
            if (count > 5 * current_state.get_max_size()) throw "high count";
            
            next_node = randint(G.num_nodes());
        }
    }
    
    else {
        next_node = get_random_item(nbtw_neighbors);
    }
    
    current_state.update_history(next_node);
}

