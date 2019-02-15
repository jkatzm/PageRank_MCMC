//
//  main.cpp
//  C++HOP_expander
//
//  Created by Katz, Jordan Alexander on 1/30/19.
//  Copyright © 2019 Katz, Jordan Alexander. All rights reserved.
//

#include "walk.hpp"
#include <iostream>
#include <cassert>
#include <chrono>


int main(int argc, const char * argv[]) {

    const string file_dir = "/p/lscratchh/katz13/Data/"; // "/Users/katz13/Dropbox/C++HOP_expander/Data/";
    const string file_name = "SNAP_Higgs_Twitter/JKATZM_higgs_influence_network.txt";
    // files:
    // SNAP_CollegeMsg_adjusted.txt
    // SNAP_facebook_0.txt
    // SNAP_Higgs_Twitter/JKATZM_higgs_influence_network.txt
    
    const string edge_list_type = "nnt";
    // edge list types:
    // nn
    // nnt
    // nlnl
    
    cout << "Building graph " << file_name << "...\n";
    graph G(file_dir + file_name, edge_list_type); // Parameter
    cout << "Graph has " << G.num_nodes() << " nodes\n";
    
    string rule = "temporally_causal";
    // transition rules:
    // PageRank
    // temporally_causal
    // self_avoiding
    // temporal_self_avoiding
    
    transition_rule T(rule); // Parameter
    cout << "Transition rule: \"" + rule + "\"\n";
    
    const int history_size = 1; // Parameter
    cout << "History size: " << history_size << "\n";
    
    const double alpha = 0.85; // Parameter
    cout << "alpha: " << alpha << "\n";
    
    const int num_steps = 1; // Parameter
    cout << "num_steps/node: " << num_steps << "\n";
    
    const bool global = true; // Parameter
    cout << "Global ranking: " << std::boolalpha << global << "\n";
    
    const node_id seed_node = -1; // Parameter
    cout << "Seed node: " << seed_node << "\n";
    
    const node_id target_node = -1; // Parameter
    cout << "Target node: " << target_node << "\n";
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

    auto start_time = chrono::high_resolution_clock::now();
    
    cout << "\nRunning simulation...\n";
    const score& CHOP_scores = simulate_walk(G, T, seed_node, history_size, alpha, num_steps, global);
    cout << "Sumulation complete.\n";
    
    // cout << CHOP_scores.CP_scores.at(target_node) << " " << CHOP_scores.EP_scores.at(target_node) << '\n';
    
    cout << "\nGlobal CP scores:\n";
    print_top_k_scores(CHOP_scores.CP_scores, int(G.num_nodes()));

    cout << "\nGlobal EP scores:\n";
    print_top_k_scores(CHOP_scores.EP_scores, int(G.num_nodes()));
    
    auto stop_time = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::seconds>(stop_time - start_time);
    cout << elapsed.count() << " seconds elapsed\n";
    return 0;
}



