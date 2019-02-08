//
//  graph.cpp
//  C++HOP_expander
//
//  Created by Katz, Jordan Alexander on 2/4/19.
//  Copyright © 2019 Katz, Jordan Alexander. All rights reserved.
//

#include "graph.hpp"
#include <cassert>
#include <iostream>
#include <fstream>
#include <algorithm> // find

graph::graph(string filename, string data_format, bool undir) {
    node_id i, j;
    node_label label_i, label_j;
    timestamp t;
    
    ifstream ifs;
    ifs.open(filename, ifstream::in);
    
    if (data_format == "nn") { // node1 node2
        while (ifs >> i >> j) {
            out_neighbors[i].push_back(j);
            
            if (undir) {
                out_neighbors[j].push_back(i);
            }
            else {
                out_neighbors[j];
            }
        }
    }
    
    else if (data_format == "nlnl") { // node1 label1 node2 label2
        while (ifs >> i >> label_i >> j >> label_j ) {
            out_neighbors[i].push_back(j);
            
            if (undir) {
                out_neighbors[j].push_back(i);
            }
            else {
                out_neighbors[j];
            }
            
            labels[i] = label_i;
            labels[j] = label_j;
        }
    }
    
    else if (data_format == "nnt") { // node1 node2 timestamp
        while (ifs >> i >> j >> t) {
            out_edges[i].emplace_back(out_edge(j, t));
            
            if (undir) {
                out_edges[j].emplace_back(out_edge(i, t));
            }
            else {
                out_edges[j];
            }
        }
    }
    
    else {
        ifs.close();
        throw "unknown graph type";
    }
    
    ifs.close();
}



bool graph::has_timestamps() const {
    return !out_edges.empty();
}

bool graph::has_labels() const {
    return !labels.empty();
}



size_t graph::num_nodes() const {
    if (has_timestamps()) {
        return out_edges.size();
    }
    return out_neighbors.size();
}

bool graph::node_exists(node_id n) const {
    if (has_timestamps()) {
        return out_edges.find(n) != out_edges.end();
    }
    return out_neighbors.find(n) != out_neighbors.end();
}



node_label graph::get_label(node_id n) const {
    assert(node_exists(n));
    return labels.at(n);
}

const vector<node_id>& graph::get_out_neighbors(node_id n) const {
    assert(node_exists(n));
    return out_neighbors.at(n);
}

const vector<out_edge>& graph::get_out_edges(node_id n) const {
    assert(node_exists(n));
    return out_edges.at(n);
}




void graph::print_neighbors(node_id n) const {
    cout << n << "\n";
    
    if (has_timestamps()) {
        for (const auto& it : get_out_edges(n)) {
            cout << "--> " << it.n << " (" << it.t << ")\n";
        }
    }
    
    else {
        for (const auto& it : get_out_neighbors(n)) {
            cout << "--> " << it << "\n";
        }
    }
}

void graph::print_graph() const {
    for (node_id n = 0; n < num_nodes(); ++n) {
        print_neighbors(n);
    }
}



