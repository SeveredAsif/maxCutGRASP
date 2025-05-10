
#ifndef SEMI_GREEDY_HPP
#define SEMI_GREEDY_HPP
#include "rcl_based.hpp"
#include "grasp.hpp" 

// Forward declarations
pair<pair<vector<bool>, vector<bool>>, int> local_search(vector<bool> &X, vector<bool> &Y, Graph &g);

int semiGreedy( Graph &g)
{
    list<int> *adj = g.getEdge();
    list<int> *adjWeights = g.getWeights();

    pair<vector<bool>, vector<bool>> bestPartition;
    int size = g.getSize();
    pair<vector<bool>, vector<bool>> p = rcl_algorithm(g, size, 0.5);
    vector<bool> X = p.first;
    vector<bool> Y = p.second;

    int w_x = calculate_cut_weight(X, Y, adj, adjWeights, size);

    return w_x;
}

#endif