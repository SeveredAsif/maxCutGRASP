// reportGenerator.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include "graph.hpp"
#include "randomized.hpp"     // double randomized(Graph&, int)
#include "greedy_max_cut.hpp" // pair<vector<bool>,vector<bool>> greedyMaxCut(Graph&, int)
#include "local-search.hpp"   // pair<vector<bool>,vector<bool>> local_search(vector<bool>&, vector<bool>&, Graph&)
#include "grasp.hpp"          // int grasp(int, Graph&)      // int calculate_cut_weight(...)

using namespace std;

int main()
{
    vector<string> names = {
        "G1", "G2", "G3", "G11", "G12", "G13",
        "G14", "G15", "G16", "G22", "G23", "G24",
        "G32", "G33", "G34", "G35", "G36", "G37",
        "G43", "G44", "G45", "G48", "G49", "G50"};

    vector<int> known = {
        12078, 12084, 12077, 627, 621, 645,
        3187, 3169, 3172, 14123, 14129, 14131,
        1560, 1537, 1541, 8000, 7996, 8009,
        7027, 7022, 7020, 6000, 6000, 5988};

    constexpr int RAND_ITERS = 1;   // as before
    constexpr int LOCAL_ITERS = 10; // e.g. 10 restarts of local
    constexpr int GRASP_ITERS = 5;  // e.g. 5 GRASP iterations

    ofstream csv("report.csv");
    csv << "Name,|V|,|E|,Rand1,Greedy1,Local1_Best,Local1_Iter,GRASP_Best,GRASP_Iter,KnownBest\n";

    for (int idx = 0; idx < (int)names.size(); ++idx)
    {
        // --- read graph ---
        string id = names[idx];
        freopen(("./input/" + id + ".rud").c_str(), "r", stdin);
        int n, m;
        cin >> n >> m;
        Graph g(n);
        for (int i = 0; i < m; i++)
        {
            int u, v, w;
            cin >> u >> v >> w;
            g.addEdge(u, v, w);
        }

        // 1) Randomized-1
        double randVal = randomized(g, RAND_ITERS);

        // 2) Greedy-1
        auto greedyP = greedyMaxCut(g, n);
        int greedyVal = calculate_cut_weight(greedyP.first,
                                             greedyP.second,
                                             g.getEdge(),
                                             g.getWeights(),
                                             n);

        // 3) Simple Local (multi-restart)
        int bestLocalVal = numeric_limits<int>::min();
        int bestLocalIter = -1;
        for (int it = 1; it <= LOCAL_ITERS; ++it)
        {
            // start from greedy each time
            auto startP = greedyMaxCut(g, n);
            auto localP = local_search(startP.first,
                                       startP.second,
                                       g);
            int val = calculate_cut_weight(localP.first,
                                           localP.second,
                                           g.getEdge(),
                                           g.getWeights(),
                                           n);
            if (val > bestLocalVal)
            {
                bestLocalVal = val;
                bestLocalIter = it;
            }
        }

        // 4) GRASP (multi-iteration)
        int bestGraspVal = numeric_limits<int>::min();
        int bestGraspIter = -1;
        for (int it = 1; it <= GRASP_ITERS; ++it)
        {
            int val = grasp(1, g); // one GRASP iteration returns w(x)
            if (val > bestGraspVal)
            {
                bestGraspVal = val;
                bestGraspIter = it;
            }
        }

        // 5) write CSV
        csv << id << ","
            << n << ","
            << m << ","
            << randVal << ","
            << greedyVal << ","
            << bestLocalVal << ","
            << LOCAL_ITERS << ","
            << bestGraspVal << ","
            << GRASP_ITERS << ","
            << known[idx]
            << "\n";
    }

    csv.close();
    cout << "Done! report.csv generated.\n";
    return 0;
}
