// iterate over V' and calculate the greedy value and save to m
// 11:52 pm - 12:35 - (paused)
// 9:59 am (next day) - 11:07
// 11:07 - 12:43 - fixing - 1.5 hours
#ifndef RCL_BASED_HPP
#define RCL_BASED_HPP

#include "graph.hpp"
#include <set>
#include <algorithm>
#include <vector>
#include <chrono>
#include <iostream>
#include <climits>
#include <list>
using namespace std;

// Forward declaration of functions
void rclBased(vector<bool> &X, vector<bool> &Y, list<int> *&adj, list<int> *&adjWeights, vector<bool> &U, int V, int alpha, int &counter, vector<int> &sigmaX, vector<int> &sigmaY);

pair<vector<bool>, vector<bool>> rcl_algorithm(Graph &g, int V, int alpha);

void rclBased(vector<bool> &X, vector<bool> &Y, list<int> *&adj, list<int> *&adjWeights, vector<bool> &U, int V, int alpha, int &counter, vector<int> &sigmaX, vector<int> &sigmaY)
{

    // Find min and max over only valid vertices - don't need to scan all vertices
    int wmin = INT_MAX;
    int wmax = INT_MIN;

    for (int v = 0; v < V; ++v)
    {
        if (!U[v])
            continue;

        int best = max(sigmaX[v], sigmaY[v]);
        wmin = min(wmin, best);
        wmax = max(wmax, best);
    }

    // Calculate threshold for RCL
    double mu = wmin + alpha * (wmax - wmin);

    // Build RCL vector directly while checking threshold
    vector<int> rclVec;
    rclVec.reserve(counter); // Preallocate for efficiency

    for (int v = 0; v < V; ++v)
    {
        if (U[v] && max(sigmaX[v], sigmaY[v]) >= mu)
        {
            rclVec.push_back(v);
        }
    }

    if (rclVec.empty())
        return;

    // Choose a random element from RCL
    int choose = rand() % rclVec.size();
    int chosenItem = rclVec[choose];

    // Decide which set to put it in
    bool addToX = false;
    if (sigmaX[chosenItem] >= sigmaY[chosenItem])
    {
        Y[chosenItem] = true;
    }
    else
    {
        X[chosenItem] = true;
        addToX = true;
    }

    // Remove from eligible set
    U[chosenItem] = false;
    counter--;

    // Key optimization: Incrementally update sigma values for remaining vertices
    for (int v = 0; v < V; ++v)
    {
        if (!U[v])
            continue;

        // Check if v is adjacent to chosenItem
        auto itr = std::find(adj[v].begin(), adj[v].end(), chosenItem);
        if (itr != adj[v].end())
        {
            // Find weight
            auto wPos = adjWeights[v].begin();
            std::advance(wPos, std::distance(adj[v].begin(), itr));
            int weight = *wPos;

            // Update sigma values based on where chosenItem was placed
            if (addToX)
            {
                sigmaX[v] += weight;
            }
            else
            {
                sigmaY[v] += weight;
            }
        }
    }
}

std::pair<std::vector<bool>, std::vector<bool>> rcl_algorithm(Graph &g, int V, int alpha)
{
    // Initialize partitions
    std::vector<bool> X(V, false);
    std::vector<bool> Y(V, false);

    // Find edge with maximum weight for initial assignment
    std::list<int> *adj = g.getEdge();
    std::list<int> *adjWeights = g.getWeights();
    int maxWeight = 0;
    int u = 0, v = 0;

    for (int i = 0; i < V; i++)
    {
        auto itr = adj[i].begin();
        auto ptr = adjWeights[i].begin();
        while (itr != adj[i].end() && ptr != adjWeights[i].end())
        {
            if (*ptr > maxWeight)
            {
                u = i;
                v = *itr;
                maxWeight = *ptr;
            }
            ++itr;
            ++ptr;
        }
    }

    // Place u and v in different partitions
    X[u] = true;
    Y[v] = true;

    // Initialize set of unassigned vertices
    std::vector<bool> U(V, true);
    U[u] = false;
    U[v] = false;
    int U_true_counter = V - 2;

    // Initialize sigma values once instead of recalculating
    std::vector<int> sigmaX(V, 0);
    std::vector<int> sigmaY(V, 0);

    // Populate initial sigmaX and sigmaY values
    for (int vertex = 0; vertex < V; ++vertex)
    {
        if (!U[vertex])
            continue;

        auto itr = adj[vertex].begin();
        auto ptr = adjWeights[vertex].begin();

        while (itr != adj[vertex].end() && ptr != adjWeights[vertex].end())
        {
            if (X[*itr])
            {
                sigmaX[vertex] += *ptr;
            }
            else if (Y[*itr])
            {
                sigmaY[vertex] += *ptr;
            }
            ++itr;
            ++ptr;
        }
    }

    // Main loop
    while (U_true_counter > 0)
    {
        rclBased(X, Y, adj, adjWeights, U, V, alpha, U_true_counter, sigmaX, sigmaY);
    }

    return {X, Y};
}

#endif // RCL_BASED_HPP