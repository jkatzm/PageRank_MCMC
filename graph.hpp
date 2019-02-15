//
//  graph.hpp
//  C++HOP_expander
//
//  Created by Katz, Jordan Alexander on 2/4/19.
//  Copyright © 2019 Katz, Jordan Alexander. All rights reserved.
//

#ifndef graph_hpp
#define graph_hpp

#include <string>
#include <unordered_map>
#include <map>
#include <vector>

using namespace std;
using node_id = int;
using node_label = int; // TODO: make it so that "label" can be of any type
using timestamp = int;

struct out_edge {
    out_edge(const node_id node, const timestamp time = -1) : n(node), t(time) {} // constructopr
    
    out_edge(const out_edge& old_obj) : n(old_obj.n), t(old_obj.t) {} // copy constructor
    
    out_edge& operator=(out_edge&&) { // move assignment
        return *this;
    }
    
    bool operator< (const out_edge& other) const {
        return (t < other.t);
    }

    const node_id n;
    const timestamp t;
};

struct graph {
    // ASSUMPTION: the nodes should be labeled 0 to |V|-1
    
    graph(string filename, string data_format, bool undir = false);
    
    bool has_timestamps() const;
    bool has_labels() const;
    
    size_t num_nodes() const;
    bool node_exists(node_id n) const;
    
    node_label get_label(node_id n) const;
    const vector<node_id>& get_out_neighbors(node_id n) const;
    const vector<out_edge>& get_out_edges(node_id n) const;
    
    void print_neighbors(node_id n) const;
    void print_graph() const;

private:
    unordered_map<node_id, vector<node_id>> out_neighbors; // maps nodes to their out-neighbors
    unordered_map<node_id, vector<out_edge>> out_edges; // maps nodes to the out-edges (sorted by timestamp) they participate in
    unordered_map<node_id, node_label> labels; // maps nodes to their labels
};


#endif /* graph_hpp */
