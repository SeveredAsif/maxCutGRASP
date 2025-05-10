// 4:37 - 5:09
// 5:09 - 5:19 - test and check
// set - vec - 7:55 -
#include "local-search.hpp"
#include "rcl_based.hpp"
using namespace chrono;

int calculate_cut_weight(const vector<bool> &X, const vector<bool> &Y, list<int> *adj, list<int> *adjWeights, int V)
{
    int total_weight = 0;

    for (int u = 0; u < V; u++)
    {
        if (!X[u])
            continue; // Only process vertices in X
        auto itr = adj[u].begin();
        auto wtr = adjWeights[u].begin();

        while (itr != adj[u].end() && wtr != adjWeights[u].end())
        {
            int v = *itr;
            int weight = *wtr;

            if (Y[*itr])
            {
                total_weight += weight;
            }

            ++itr;
            ++wtr;
        }
    }

    return total_weight;
}

// Forward declarations
pair<vector<bool>, vector<bool>> local_search(vector<bool> &X, vector<bool> &Y, Graph &g);

int grasp(int iterations, Graph &g)
{
    list<int> *adj = g.getEdge();
    list<int> *adjWeights = g.getWeights();
    int best_Wx = 0;
    for (int i = 0; i < iterations; i++)
    {
        auto iter_start = high_resolution_clock::now();
        auto t1 = high_resolution_clock::now();
        pair<vector<bool>, vector<bool>> bestPartition;
        int size = g.getSize();
        pair<vector<bool>, vector<bool>> p = rcl_algorithm(g, size, 0.5);
        auto t2 = high_resolution_clock::now();
        vector<bool> X = p.first;
        vector<bool> Y = p.second;
        auto t3 = high_resolution_clock::now();
        p = local_search(X, Y, g);
        auto t4 = high_resolution_clock::now();
        if (i == 0)
            bestPartition = p;

        int w_x = calculate_cut_weight(X, Y, adj, adjWeights, size);

        // Timing durations
        auto dur_rcl = duration_cast<milliseconds>(t2 - t1).count();
        auto dur_local = duration_cast<milliseconds>(t4 - t3).count();

        cout << "Iteration " << i + 1 << ": "
             << "rcl_algorithm = " << dur_rcl << " ms, "
             << "local_search = " << dur_local << " ms" << endl;
        if (w_x > best_Wx)
        {
            best_Wx = w_x;
        }
    }
    return best_Wx;
}