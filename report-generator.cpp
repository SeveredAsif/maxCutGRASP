#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include "graph.hpp"
#include "randomized.hpp"     // double randomized(Graph&, int)
#include "greedy_max_cut.hpp" // pair<set<int>,set<int>> greedyMaxCut(Graph&, int)
#include "local-search.hpp"   // pair<set<int>,set<int>> local_search(set<int>&, set<int>&, Graph&)
#include "grasp.hpp"          // int grasp(int, Graph&)

using namespace std;

// Forward declaring utility to compute cut-weight from partition
// int calculate_cut_weight(const vector<bool> &X, const vector<bool> &Y, list<int> *adj, list<int> *adjWeights, int V)
// {
//     int total_weight = 0;

//     for (int u = 0; u < V; u++)
//     {
//         if (!X[u])
//             continue; // Only process vertices in X
//         auto itr = adj[u].begin();
//         auto wtr = adjWeights[u].begin();

//         while (itr != adj[u].end() && wtr != adjWeights[u].end())
//         {
//             int v = *itr;
//             int weight = *wtr;

//             if (Y[*itr])
//             {
//                 total_weight += weight;
//             }

//             ++itr;
//             ++wtr;
//         }
//     }

//     return total_weight;
// }

int main()
{
    // Graph identifiers and known best solutions
    vector<string> names = {"G1" /*, "G2", "G3", "G11", "G12", "G13",
                             "G21", "G22", "G23", "G31", "G32", "G33",
                             "G41", "G42", "G43", "G51", "G52", "G53",
                             "G61", "G62", "G63", "G71", "G72", "G73"*/
    };
    vector<int> knownBest = {12078 /*,14123, 7027, /* ... fill for 24 graphs ...*/};

    ofstream csv("report.csv");
    // Header
    csv << "Name,|V|,|E|,Randomized-1,Greedy-1,Local-1,GRASP-1,KnownBest\n";

    for (size_t i = 0; i < names.size(); ++i)
    {
        string id = names[i];
        string path = "./input/" + id + ".rud";
        freopen(path.c_str(), "r", stdin);

        int n, m;
        cin >> n >> m;
        Graph g(n);
        for (int e = 0; e < m; ++e)
        {
            int u, v, w;
            cin >> u >> v >> w;
            g.addEdge(u, v, w);
        }

        // 1) Randomized-1 (single trial average)
        double randVal = randomized(g, 1);

        // 2) Greedy-1
        auto greedyP = greedyMaxCut(g, n);
        auto adj = g.getEdge();
        auto adjwt = g.getWeights();
        int greedyVal = calculate_cut_weight(greedyP.first, greedyP.second, adj, adjwt, g.getSize());

        // 3) Local-1: apply local_search on greedy
        vector<bool> X = greedyP.first, Y = greedyP.second;
        auto localP = local_search(X, Y, g);
        int localVal = calculate_cut_weight(localP.first, localP.second, adj, adjwt, g.getSize());

        // 4) GRASP-1 (single iteration)
        int graspVal = grasp(1, g);

        // Write CSV line
        csv << id << ","
            << n << ","
            << m << ","
            << randVal << ","
            << greedyVal << ","
            << localVal << ","
            << graspVal << ","
            << knownBest[i] << "\n";
    }

    csv.close();
    cout << "Report written to report.csv\n";
    return 0;
}
