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

void print_top_k_scores(const vector<double>& scores, int k) {
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

int main(int argc, const char * argv[]) {
    const string file_name = "SNAP_facebook_0.txt"; // Pseudo-Parameter
    // files:
    // CollegeMsg_adjusted.txt
    // SNAP_facebook_0.txt
    
    transition_rule T("self_avoiding"); // Parameter
    // transition rules:
    // PageRank
    // temporally_causal
    // self_avoiding
    
    const string edge_list_type = "nn"; // Pseudo-Parameter
    // edge list types:
    // nn
    // nnt
    // nlnl
    
    const string file_dir = "/Users/katz13/Dropbox/C++HOP_expander/Data/";
    graph G(file_dir + file_name, edge_list_type);
    
    const int history_size = 3; // Parameter
    const double alpha = 0.85; // Parameter
    const node_id seed = 0; // Parameter
    const int num_steps = 50000; // Parameter
    const node_id target_node = 20; // Parameter
    
    cout << "EP\tCP\n";
    
    for (int experiment = 0; experiment < 20; ++experiment) {
        const auto& scores = simulate_walk(G, T, seed, history_size, alpha, num_steps);
        
//        print_top_k_scores(scores.CP_scores, 20);
//        print_top_k_scores(scores.EP_scores, 20);
        
        cout << scores.CP_scores.at(target_node) << "\t" << scores.EP_scores.at(target_node) << "\n";
    }
    
    return 0;
}



