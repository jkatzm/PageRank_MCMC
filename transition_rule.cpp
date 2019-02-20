//
//  transition_rule.cpp
//  C++HOP_expander
//
//  Created by Katz, Jordan Alexander on 2/4/19.
//  Copyright © 2019 Katz, Jordan Alexander. All rights reserved.
//

#include "transition_rule.hpp"
#include <iostream>

int randint(size_t upper_bound, std::mt19937& engine) {
    std::uniform_int_distribution<int> dist(0, int(upper_bound)-1);
    return dist(engine);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

template <typename T>
const T& get_random_item(const vector<T>& vec, std::mt19937& engine) {
    return vec.at(randint(vec.size(), engine));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

void transition_rule::operator()(const graph& G, state& current_state, std::mt19937& engine) const {
    if (this->rule == "PageRank") {
        T_PageRank(G, current_state, engine);
        // CAUTION: see below
    }
    else if (this->rule == "temporally_causal") {
        assert(G.has_timestamps());
        T_temporally_causal(G, current_state, engine);
    }
    else if (this->rule == "self_avoiding") {
        assert(G.has_timestamps() == false);
        T_self_avoiding(G, current_state, engine);
    }
    else if (this->rule == "temporal_self_avoiding") {
        assert(G.has_timestamps());
        T_temporal_self_avoiding(G, current_state, engine);
    }
    else throw "unknown rule";
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

void transition_rule::T_PageRank(const graph& G, state& current_state, std::mt19937& engine) const {
    node_id next_node = -1;
    
    if (G.has_timestamps()) {
        const vector<out_edge>& neighbors = G.get_out_edges(current_state.get_current_node());
        
        if (neighbors.empty() == false) {
            next_node = get_random_item(neighbors, engine).n;
            // CAUTION: this is not a uniform selection... there could be more than edge pointing to the same node
        }
    }
    
    else {
        const vector<node_id>& neighbors = G.get_out_neighbors(current_state.get_current_node());
        
        if (neighbors.empty() == false) {
            next_node = get_random_item(neighbors, engine);
        }

    }
    
    if (next_node == -1) {
        next_node = randint(G.num_nodes(), engine);
    }

    current_state.update_history(next_node);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

void transition_rule::T_temporally_causal(const graph& G, state& current_state, std::mt19937& engine) const {
    const vector<out_edge>& neighbors = G.get_out_edges(current_state.get_current_node());
    // assumes neighbors are sorted by timestamp
    
    auto comp = [=](const out_edge& e) { return e.t > current_state.get_previous_timestamp(); };
    auto first_causal = find_if(neighbors.begin(), neighbors.end(), comp);
    size_t distance = std::distance(first_causal, neighbors.end());

    if (distance == 0) { // dangling
        node_id next_node = randint(G.num_nodes(), engine);
        current_state.update_history(next_node);
    }

    else {
        auto next = first_causal + randint(distance, engine);
        current_state.update_history(next->n);
        current_state.update_timestamp(next->t);
    }
    
//    // get the neighbors whose edges have a timestamp >= prev timestamp
//    node_id next_node;
//    vector<out_edge> causal_neighbors;
//    causal_neighbors.reserve(neighbors.size());
//
//    for (const auto& it : neighbors) {
//        if (it.t >= current_state.get_previous_timestamp()) {
//            causal_neighbors.push_back(it);
//        }
//    }
//
//    // pick a random causal neighbor to jump to
//    // if there are none, pick a random node and keep the previous timestamp the same
//    if (causal_neighbors.empty()) {
//        next_node = randint(G.num_nodes(), engine);
//
//    }
//
//    else {
//        const out_edge& edge = get_random_item(causal_neighbors, engine);
//        current_state.update_timestamp(edge.t);
//        next_node = edge.n;
//    }
//
//    current_state.update_history(next_node);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

void transition_rule::T_self_avoiding(const graph& G, state& current_state, std::mt19937& engine) const {
    node_id next_node;
    const vector<node_id>& neighbors = G.get_out_neighbors(current_state.get_current_node());
    
    // get the neighbors who aren't in the current history (NSI = non-self-intersecting)
    vector<node_id> NSI_neighbors;
    NSI_neighbors.reserve(neighbors.size());
    
    for (auto it : neighbors) {
        if (current_state.does_not_contain(it)) {
            NSI_neighbors.push_back(it);
        }
    }
    
    // pick a random NSI neighbor to jump to
    // if there are none, pick a node not in the current history
    if (NSI_neighbors.empty()) {
        int count = 0;
        
        next_node = randint(G.num_nodes(), engine);
        
        while (current_state.contains(next_node)) { // rejection sampling
            count++;
            
            if (count > 5 * current_state.get_max_size()) throw "high count";
            
            next_node = randint(G.num_nodes(), engine);
        }
    }
    
    else {
        next_node = get_random_item(NSI_neighbors, engine);
    }
    
    current_state.update_history(next_node);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

void transition_rule::T_temporal_self_avoiding(const graph& G, state& current_state, std::mt19937& engine) const {
    node_id next_node;
    const vector<out_edge>& neighbors = G.get_out_edges(current_state.get_current_node());
    
    /*******
    TODO
    *******/
    
    // Assumption: neighbors are sorted by timestamp!!
    auto comp = [=](const out_edge& e) { return e.t >= current_state.get_previous_timestamp(); };
    


    auto it = find_if(neighbors.begin(), neighbors.end(), comp);
    
    size_t distance = std::distance(it, neighbors.end());
    
    if (distance == 0) { // dangling
        next_node = randint(G.num_nodes(), engine);
        current_state.update_history(next_node);
    }
    
    // TODO: check to make sure none of the remaining nodes are in the current history
    
    else {
        auto next = it + randint(distance, engine);
        
        int count = 0;
        while (current_state.contains(next->n)) {
            count++;
            if (count > 30) {
                throw "shit";
            }
            next = it + randint(distance, engine);
        }
        
        current_state.update_history(next->n);
        current_state.update_timestamp(next->t);
    }
    
    /*******
     TODO
     *******/
    
    /*
    
    // get the neighbors whose edges have a timestamp >= prev timestamp AND are not currently in the history
    vector<out_edge> causal_NSI_neighbors;
    causal_NSI_neighbors.reserve(neighbors.size());
    
    for (const auto& it : neighbors) {
        if (it.t >= current_state.get_previous_timestamp() && current_state.does_not_contain(it.n)) {
            causal_NSI_neighbors.push_back(it);
        }
    }
    
    // pick a random  neighbor to jump to
    // if there are none, pick a random node and keep the previous timestamp the same
    if (causal_NSI_neighbors.empty()) {
        next_node = randint(G.num_nodes(), engine);
        // TODO: change this to be more like the NSI dangling nodes?
    }
    
    else {
        const out_edge& edge = get_random_item(causal_NSI_neighbors, engine);
        current_state.update_timestamp(edge.t);
        next_node = edge.n;
    }
    
    current_state.update_history(next_node);
    */
}
