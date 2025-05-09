// 9:41 am-10:02 pm (not ended - 19 min)
//10:18 - 10:35 - 17 min
//checking : 10:36 - 11:04 - 28 min 
//total 1 hr 4 min 
#include <set>
#include <list>
#include <utility>
#include "graph.hpp"
using namespace std;
pair<set<int>,set<int>> greedyMaxCut(Graph &g, int V)
{
    pair<set<int>,set<int>>ans;
    // initializing partitions
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
    } // till here 10:02
    // 10:18 - 10:35

    // compute wx and wy
    for (auto x : U)
    {
        int w_x = 0;
        int w_y = 0;
        auto itr = adj[x].begin();
        auto ptr = adjWeights[x].begin();
        while (itr != adj[x].end() && ptr != adjWeights[x].end())
        {
            // finding the element in the set for wzy if the element is in Y
            if (Y.find(*itr) != Y.end())
            {
                w_x += (*ptr);
            }

            // computing wy if the element is in X
            if (X.find(*itr) != X.end())
            {
                w_y += (*ptr);
            }
            ++itr;
            ++ptr;
        }
        // compare wx and wy
        if (w_x > w_y)
        {
            X.insert(x);
        }
        else{
            Y.insert(x);
        }
    }
    ans = {X,Y};
    return ans;
}