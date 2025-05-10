// 9:41 am-10:02 pm (not ended - 19 min)
// 10:18 - 10:35 - 17 min
// checking : 10:36 - 11:04 - 28 min
// total 1 hr 4 min
#include <vector>
#include <list>
#include <utility>
#include "graph.hpp"
using namespace std;
pair<vector<bool>, vector<bool>> greedyMaxCut(Graph &g, int V)
{
    pair<vector<bool>, vector<bool>> ans;
    // initializing partitions
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
    X[u] = true;
    Y[v] = true;
    // initialize U
    vector<bool> U (V,false);
    for (int i = 0; i < V; i++)
    {
        // if i is not u, i is not v, push it to set U
        if (i != u && i != v)
        {
            U[i]=true;
        }
    } // till here 10:02
    // 10:18 - 10:35

    // compute wx and wy
    // wx is, if a node is in X, the weight added extra
    for (int i=0;i<V;i++)
    {   
        if(!U[i]) continue;
        int w_x = 0;
        int w_y = 0;
        auto itr = adj[i].begin();
        auto ptr = adjWeights[i].begin();
        while (itr != adj[i].end() && ptr != adjWeights[i].end())
        {
            // finding the element in the set for wzy if the element is in Y
            if (Y[*itr])
            {
                w_x += (*ptr);
            }

            // computing wy if the element is in X
            if (X[*itr])
            {
                w_y += (*ptr);
            }
            ++itr;
            ++ptr;
        }
        // compare wx and wy
        if (w_x > w_y)
        {
            X[i] = true;
        }
        else
        {
            Y[i] = true;
        }
    }
    ans = {X, Y};
    return ans;
}