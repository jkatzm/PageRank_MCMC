//
//  state.hpp
//  C++HOP_expander
//
//  Created by Katz, Jordan Alexander on 2/4/19.
//  Copyright © 2019 Katz, Jordan Alexander. All rights reserved.
//

#ifndef state_hpp
#define state_hpp

#include <deque>
#include <algorithm> // find
#include <cassert>
#include <iostream>

using namespace std;
using node_id = int;
using timestamp = int;

struct state {
    state(int max_size, node_id initial_node) : k(max_size), prev_timestamp(-1) {
        history.push_back(initial_node);
        assert(max_size >= 1);
    }
    
    
    
    int get_max_size() {
        return k;
    }
    
    timestamp get_previous_timestamp() const {
        return prev_timestamp;
    }
    
    node_id get_current_node() const {
        return history.back();
    }
    
    void print_state() {
        for (node_id it : history) {
            cout << it << " ";
        }
        cout << "\n";
    }
    
    
    
    void update_history(node_id next_node) {
        history.push_back(next_node);
        
        if (history.size() > k) {
            history.pop_front();
        }
        assert(history.size() <= k);
    }
    
    void update_timestamp(int next_t) {
        prev_timestamp = next_t;
    }
    
    
    
    bool contains(node_id n) {
        return std::find(history.begin(), history.end(), n) != history.end();
    }
    
    bool does_not_contain(node_id n) {
        return !contains(n);
    }
    
    

    
private:
    const int k; // max allotted history size
    deque<node_id> history; // takes into consideration the "current node"
    timestamp prev_timestamp;
};

#endif /* state_hpp */
