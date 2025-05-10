// 12:46 pm - 1:22 (BREAK);
// 2:09-2:10 (pause)
// 2:48 - 3: 14
// checking 3:15 -
#include <set>
#include <vector>
#include "graph.hpp"
using namespace std;

pair<int, int> calculateSigma(int v, list<int> *&adj, list<int> *&adjWeights, set<int> &inSet, set<int> &outSet)
{
    int inSum = 0;
    int outSum = 0;

    auto itr = adj[v].begin();
    auto ptr = adjWeights[v].begin();
    while (itr != adj[v].end() && ptr != adjWeights[v].end())
    {
        if (outSet.find(*itr) != outSet.end())
        {
            outSum += (*ptr);
        }

        // computing sigmaY if the element is in X
        if (inSet.find(*itr) != inSet.end())
        {
            inSum += (*ptr);
        }
        ++itr;
        ++ptr;
    }
    return {inSum, outSum};
}

pair<set<int>, set<int>> local_search(set<int> &X, set<int> &Y, Graph &g)
{
    set<int> S;
    set<int> S_p;
    for (auto i : X)
    {
        S.insert(i);
    }
    for (auto i : Y)
    {
        S_p.insert(i);
    }

    list<int> *adj = g.getEdge();
    list<int> *adjWeights = g.getWeights();

    // repeat until stuck in local optima
    while (1)
    {
        int bestV = -1;
        int max = 0;
        bool is_S = true;
        bool hasImproved = false;

        // for values in S, move it to S' and see if values improved
        //  note down the value v for which the sigma change is the most
        // insum = sum of weights between v and its own set (meaning insum = what it would be if i moved v to the other set)
        // outsum = sum of wt btn v and the other set values 
        // insum - outsum gives == move and get weights - current get weights 
        for (auto v : S)
        {
            pair<int, int> p = calculateSigma(v, adj, adjWeights, S, S_p);
            int inSum = p.first;
            int outSum = p.second;
            if (inSum - outSum > 0)
            {
                if (inSum - outSum > max)
                {
                    max = inSum - outSum;
                    bestV = v;
                    is_S = true;
                }
                hasImproved = true;
            }
        }

        // same for S'
        for (auto v : S_p)
        {
            pair<int, int> p = calculateSigma(v, adj, adjWeights, S_p, S);
            int inSum = p.first;
            int outSum = p.second;
            if (inSum - outSum > 0)
            {
                if (inSum - outSum > max)
                {
                    max = inSum - outSum;
                    bestV = v;
                    is_S = false;
                }
                hasImproved = true;
            }
        }

        // if no improvement, break
        if (!hasImproved)
            break;
        if (bestV == -1)
            break;

        // now i have best v and if it was in S or S'
        if (is_S)
        {
            S.erase(bestV);
            S_p.insert(bestV);
        }
        else
        {
            S_p.erase(bestV);
            S.insert(bestV);
        }
    }

    // now i have the refined S and S' cut
    return {S, S_p};
}