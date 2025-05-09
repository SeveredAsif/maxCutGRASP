// 11:07 -11:31 - write (24 min)
//check 11:31 - 11:49 pm -18 min 
// total - 42 min 
#include <set>
#include <list>
#include <random>
#include <time.h>
#include "graph.hpp"
using namespace std;

double random_double()
{

   // Generate a random number between 0 and 100
    int randomNum = rand() % 101;
    double ans = randomNum / 100.0;
    return ans;
}

double randomized(Graph &g, int n)
{
    int totalCutWeight = 0;
    // iterations n
    for (int i = 0; i < n; i++)
    {
        int cutWeight = 0;
        set<int> X = {};
        set<int> Y = {};
        int nodes = g.getSize();
        list<int> *adj = g.getEdge();
        list<int> *adjWeights = g.getWeights();
        int maxWeight = 0;
        int u = 0;
        int v = 0;
        // randomized graph creation
        for (int i = 0; i < nodes; i++)
        {
                double randomChoice = random_double();
                if (randomChoice >= 0.5)
                {
                    X.insert(i);
                }
                else
                {
                    Y.insert(i);
                }
        }

        // calculating the graph's cut weight
        for (int i = 0; i < nodes; i++)
        {
            // checking if u is in X
            if (X.find(i) != X.end())
            {
                auto itr = adj[i].begin();
                auto ptr = adjWeights[i].begin();
                while (itr != adj[i].end() && ptr != adjWeights[i].end())
                {
                    // checking if the edge takes u to Y set
                    if (Y.find(*itr) != Y.end())
                    {
                        cutWeight += *ptr;
                    }
                    ++itr;
                    ++ptr;
                }
            }
        }
        totalCutWeight += cutWeight;
    }
    double avgCutWeight = double(totalCutWeight)/n;
    return avgCutWeight;
}
