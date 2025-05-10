// iterate over V' and calculate the greedy value and save to m
// 11:52 pm - 12:35 - (paused)
// 9:59 am (next day) - 11:07 
#include "graph.hpp"
#include <set>
#include <algorithm>

// source: https://stackoverflow.com/questions/3052788/how-to-select-a-random-element-in-stdset
template <typename S>
auto select_random(const S &s, size_t n)
{
    auto it = std::begin(s);
    // 'advance' the iterator n times
    std::advance(it, n);
    return it;
}

void rclBased(set<int> &X, set<int> &Y, list<int> *&adj, list<int> *&adjWeights, set<int> &U, int V, int alpha)
{
    // // find heaviest greedily, and put it on X , Y partition first
    // set<int> X = {};
    // set<int> Y = {};
    // // find edge u,v with maximum weight
    // list<int> *adj = g.getEdge();
    // list<int> *adjWeights = g.getWeights();
    // int maxWeight = 0;
    // int u = 0;
    // int v = 0;
    // for (int i = 0; i < V; i++)
    // {
    //     auto itr = adj[i].begin();
    //     auto ptr = adjWeights[i].begin();
    //     while (itr != adj[i].end() && ptr != adjWeights[i].end())
    //     {
    //         if (*ptr > maxWeight)
    //         {
    //             u = i;
    //             v = *itr;
    //             maxWeight = *ptr;
    //         }
    //         ++itr;
    //         ++ptr;
    //     }
    // }
    // // now i have u,v as max weighted edge
    // // insert them as the first elements in set
    // X.insert(u);
    // Y.insert(v);
    // // initialize U
    // set<int> U = {};
    // for (int i = 0; i < V; i++)
    // {
    //     // if i is not u, i is not v, push it to set U
    //     if (i != u && i != v)
    //     {
    //         U.insert(i);
    //     }
    // }

    // calculate sigmaX(v) and sigmaY(v)
    // sigmaX is , if v is in Y, then the weight added
    int *sigmaX = new int[V];
    int *sigmaY = new int[V];
    for (auto v : U)
    {
        sigmaX[v] = 0;
        sigmaY[v] = 0;
        auto itr = adj[v].begin();
        auto ptr = adjWeights[v].begin();
        while (itr != adj[v].end() && ptr != adjWeights[v].end())
        {
            if (Y.find(*itr) != Y.end())
            {
                sigmaY[v] += (*ptr);
            }

            // computing sigmaY if the element is in X
            if (X.find(*itr) != X.end())
            {
                sigmaX[v] += (*ptr);
            }
            ++itr;
            ++ptr;
        }

        // wmin and wmax

        int sigmaXMin = *min_element(sigmaX, sigmaX + V);
        int sigmaXMax = *max_element(sigmaX, sigmaX + V);
        int sigmaYMin = *min_element(sigmaY, sigmaY + V);
        int sigmaYMax = *max_element(sigmaY, sigmaY + V);
        int wmin = min(sigmaXMin, sigmaYMin);
        int wmax = max(sigmaXMax, sigmaYMax);

        double miu = wmin + alpha * (wmax - wmin);
        // calculate greedyValue
        set<int> rclSET;
        set<int> secondSet;

        for (int i = 0; i < V; i++)
        {
            int greedyVal = max(sigmaX[i], sigmaY[i]);
            if (greedyVal >= miu)
            {
                rclSET.insert(i);
            }
        }

        // choose one of the elements from the rcl set
        int choose = rand() % rclSET.size();
        int chosenItem = *select_random(rclSET, choose);

        // compare this chosenItem value in sigmaX and sigmaY
        if (sigmaX[chosenItem] >= sigmaY[chosenItem])
        {
            Y.insert(chosenItem);
        }
        else
        {
            X.insert(chosenItem);
        }

        U.erase(chosenItem);
    }
}

pair<set<int>,set<int>> rcl_algorithm(Graph &g, int V, int alpha)
{
    pair<set<int>,set<int>> ans;
    // find heaviest greedily, and put it on X , Y partition first
    set<int> X = {};
    set<int> Y = {};
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
    X.insert(u);
    Y.insert(v);

    // initialize U
    set<int> U = {};
    for (int i = 0; i < V; i++)
    {
        // if i is not u, i is not v, push it to set U
        if (i != u && i != v)
        {
            U.insert(i);
        }
    }

    while (!U.empty())
    {
        rclBased(X,Y,adj,adjWeights,U,V,alpha);
    }
    ans = {X,Y};
    return ans;
}