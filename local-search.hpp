// 12:46 pm - 1:22 (BREAK);
// 2:09-2:10 (pause)
// 2:48 - 3: 14
// checking 3:15 - 4:36 - 1 hr 15 min
// set to vector - 7:22 - 7:53 - 30 min
#ifndef LOCAL_SEARCH_HPP
#define LOCAL_SEARCH_HPP
#include <set>
#include <vector>
#include "graph.hpp"
using namespace std;

pair<int, int> calculateSigma(int v, list<int> *&adj, list<int> *&adjWeights, vector<bool> &inSet, vector<bool> &outSet)
{
    int inSum = 0;
    int outSum = 0;

    auto itr = adj[v].begin();
    auto ptr = adjWeights[v].begin();
    while (itr != adj[v].end() && ptr != adjWeights[v].end())
    {
        if (outSet[*itr])
        {
            outSum += (*ptr);
        }

        // computing sigmaY if the element is in X
        if (inSet[*itr])
        {
            inSum += (*ptr);
        }
        ++itr;
        ++ptr;
    }
    return {inSum, outSum};
}

pair<vector<bool>, vector<bool>> local_search(vector<bool> &X, vector<bool> &Y, Graph &g)
{
    // vector<bool> S(g.getSize(), false);
    // vector<bool> S_p(g.getSize(), false);
    int v = g.getSize();
    // for (auto i : X)
    // {
    //     S[i] = true;
    // }
    // for (auto i : Y)
    // {
    //     S_p[i] = true;
    // }

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
        for (int i = 0; i < v; i++)
        {
            if (!X[i])
                continue;
            pair<int, int> p = calculateSigma(i, adj, adjWeights, X, Y);
            int inSum = p.first;
            int outSum = p.second;
            if (inSum - outSum > 0)
            {
                if (inSum - outSum > max)
                {
                    max = inSum - outSum;
                    bestV = i;
                    is_S = true;
                }
                hasImproved = true;
            }
        }

        // same for S'
        for (int i = 0; i < v; i++)
        {
            if (!Y[i])
                continue;
            pair<int, int> p = calculateSigma(i, adj, adjWeights, Y, X);
            int inSum = p.first;
            int outSum = p.second;
            if (inSum - outSum > 0)
            {
                if (inSum - outSum > max)
                {
                    max = inSum - outSum;
                    bestV = i;
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
            X[bestV] = false;
            Y[bestV] = true;
        }
        else
        {
            Y[bestV] = false;
            X[bestV] = true;
        }
    }

    // now i have the refined S and S' cut
    return {X, Y};
}

#endif