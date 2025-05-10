// iterate over V' and calculate the greedy value and save to m
// 11:52 pm - 12:35 - (paused)
// 9:59 am (next day) - 11:07
// 11:07 - 12:43 - fixing - 1.5 hours
#include "graph.hpp"
#include <set>
#include <algorithm>
#include <vector>
using namespace std::chrono;

// source: https://stackoverflow.com/questions/3052788/how-to-select-a-random-element-in-stdset
template <typename S>
auto select_random(const S &s, size_t n)
{
    auto it = std::begin(s);
    // 'advance' the iterator n times
    std::advance(it, n);
    return it;
}

void rclBased(vector<bool> &X, vector<bool> &Y, list<int> *&adj, list<int> *&adjWeights, vector<bool> &U, int V, int alpha, int &counter)
{
    auto t_start = high_resolution_clock::now();
    auto t_sigma_start = high_resolution_clock::now();
    vector<int> sigmaX(V, 0);
    vector<int> sigmaY(V, 0);
    for (int v = 0; v < V; ++v)
    {
        if (!U[v])
            continue; // Skip vertices not in U
        auto itr = adj[v].begin();
        auto ptr = adjWeights[v].begin();
        while (itr != adj[v].end() && ptr != adjWeights[v].end())
        {
            if (Y[*itr])
            {
                sigmaY[v] += (*ptr);
            }

            // computing sigmaY if the element is in X
            if (X[*itr])
            {
                sigmaX[v] += (*ptr);
            }
            ++itr;
            ++ptr;
        }
    }
    // wmin and wmax
    auto t_sigma_end = high_resolution_clock::now();
    // cout << "Time for sigma computation: "
    //     << duration<double>(t_sigma_end - t_sigma_start).count() << "s\n";

    // wmin and wmax computation
    auto t_minmax_start = high_resolution_clock::now();

    int wmin = +1e9, wmax = -1e9;
    for (int v = 0; v < V; ++v)
    {
        if (!U[v])
            continue; // Skip vertices not in U
        int best = max(sigmaX[v], sigmaY[v]);
        wmin = min(wmin, best);
        wmax = max(wmax, best);
    }
    double mu = wmin + alpha * (wmax - wmin);

    // int sigmaXMin = *min_element(sigmaX.begin(), sigmaX.end());
    // int sigmaXMax = *max_element(sigmaX.begin(), sigmaX.end());
    // int sigmaYMin = *min_element(sigmaY.begin(), sigmaY.end());
    // int sigmaYMax = *max_element(sigmaY.begin(), sigmaY.end());
    // int wmin = min(sigmaXMin, sigmaYMin);
    // int wmax = max(sigmaXMax, sigmaYMax);

    // double miu = wmin + alpha * (wmax - wmin);
    // calculate greedyValue
    // Build RCL as a vector for O(1) random access
    auto t_minmax_end = high_resolution_clock::now();
    // cout << "Time for wmin/wmax computation: "
    //     << duration<double>(t_minmax_end - t_minmax_start).count() << "s\n";

    // build RCL vector
    auto t_rcl_start = high_resolution_clock::now();
    vector<int> rclVec;
    for (int v = 0; v < V; ++v)
    {
        if (U[v] && max(sigmaX[v], sigmaY[v]) >= mu)
        {
            rclVec.push_back(v);
        }
    }
    auto t_rcl_end = high_resolution_clock::now();
    // cout << "Time for RCL build: "
    //      << duration<double>(t_rcl_end - t_rcl_start).count() << "s\n";
    if (rclVec.empty())
        return;

    // set<int> rclSET;

    // for (auto i : U)
    // {
    //     int greedyVal = max(sigmaX[i], sigmaY[i]);
    //     if (greedyVal >= mu)
    //     {
    //         rclSET.insert(i);
    //     }
    // }

    // choose one of the elements from the rcl set
    auto t_choose_start = high_resolution_clock::now();
    int choose = rand() % rclVec.size();
    int chosenItem = rclVec[choose];

    // compare this chosenItem value in sigmaX and sigmaY
    if (sigmaX[chosenItem] >= sigmaY[chosenItem])
    {
        Y[chosenItem] = true;
    }
    else
    {
        X[chosenItem] = true;
    }

    U[chosenItem] = false;
    counter--;
    auto t_choose_end = high_resolution_clock::now();
    // cout << "Time for selection and update: "
    //     << duration<double>(t_choose_end - t_choose_start).count() << "s\n";

    auto t_end = high_resolution_clock::now();
    // cout << "Total iteration time: "
    //     << duration<double>(t_end - t_start).count() << "s\n";
    // cout << "------------------------------------\n";
}

pair<vector<bool>, vector<bool>> rcl_algorithm(Graph &g, int V, int alpha)
{
    pair<vector<bool>, vector<bool>> ans;
    // find heaviest greedily, and put it on X , Y partition first
    // set<int> X = {};
    // set<int> Y = {};
    vector<bool> X(V, false);
    vector<bool> Y(V, false);
    // find edge u,v with maximum weight
    list<int> *adj = g.getEdge();
    list<int> *adjWeights = g.getWeights();
    int maxWeight = 0;
    int u = 0;
    int v = 0;
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
    // now i have u,v as max weighted edge
    // insert them as the first elements in set
    // X.insert(u);
    // Y.insert(v);
    X[u] = true;
    Y[v] = true;

    // initialize U
    // set<int> U = {};
    vector<bool> U(V, false);
    int U_true_counter = 0;
    for (int i = 0; i < V; i++)
    {
        // if i is not u, i is not v, push it to set U
        if (i != u && i != v)
        {
            // U.insert(i);
            U[i] = true;
            U_true_counter++;
        }
    }

    while (U_true_counter > 0)
    {
        rclBased(X, Y, adj, adjWeights, U, V, alpha, U_true_counter);
    }
    ans = {X, Y};
    return ans;
}
