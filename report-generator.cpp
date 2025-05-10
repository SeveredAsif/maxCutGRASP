// reportGenerator.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <chrono>
#include "graph.hpp"
#include "randomized.hpp"     // double randomized(Graph&, int)
#include "greedy_max_cut.hpp" // pair<vector<bool>,vector<bool>> greedyMaxCut(Graph&, int)
#include "local-search.hpp"   // pair<vector<bool>,vector<bool>> local_search(vector<bool>&, vector<bool>&, Graph&)
#include "grasp.hpp"          // int grasp(int, Graph&)      // int calculate_cut_weight(...)

using namespace std;
using Clock = chrono::high_resolution_clock;
using ms = chrono::milliseconds;

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

    constexpr int RAND_ITERS = 1;   // number of randomized runs
    constexpr int LOCAL_ITERS = 5;  // multi-restart local-search
    constexpr int GRASP_ITERS = 25; // GRASP iterations

    ofstream csv("2105131report.csv");
    csv << "Name,|V|,|E|,"
           "Rand1,Greedy1,Local1_Best,Local1_Iter,GRASP_Best,GRASP_Iter,KnownBest\n";

    for (int idx = 0; idx < (int)names.size(); ++idx)
    {
        // --- read graph from file ---
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
        auto t0 = Clock::now();
        double randVal = randomized(g, RAND_ITERS);
        auto t1 = Clock::now();
        auto randTime = chrono::duration_cast<ms>(t1 - t0).count();
        cout << id << ",Randomized," << randTime << "ms\n";

        // 2) Greedy-1
        auto t2 = Clock::now();
        auto greedyP = greedyMaxCut(g, n);
        int greedyVal = calculate_cut_weight(
            greedyP.first,
            greedyP.second,
            g.getEdge(),
            g.getWeights(),
            n);
        auto t3 = Clock::now();
        auto greedyTime = chrono::duration_cast<ms>(t3 - t2).count();
        cout << id << ",Greedy," << greedyTime << "ms\n";

        // 3) Simple Local (multi-restart)
        auto t4 = Clock::now();
        int bestLocalVal = numeric_limits<int>::min();
        int bestLocalIter = -1;

            auto startP = greedyMaxCut(g, n);
            auto localP = local_search(startP.first,
                                       startP.second,
                                       g);
             bestLocalVal = calculate_cut_weight(
                localP.first,
                localP.second,
                g.getEdge(),
                g.getWeights(),
                n);

        auto t5 = Clock::now();
        auto localTime = chrono::duration_cast<ms>(t5 - t4).count();
        cout << id << ",LocalSearch," << localTime << "ms\n";

        // 4) GRASP (multi-iteration)
        auto t6 = Clock::now();
        int bestGraspVal = numeric_limits<int>::min();
        int bestGraspIter = -1;

        bestGraspVal = grasp(GRASP_ITERS, g);

        auto t7 = Clock::now();
        auto graspTime = chrono::duration_cast<ms>(t7 - t6).count();
        cout << id << ",GRASP," << graspTime << "ms\n";

        // 5) write CSV line
        csv << id << ","
            << n << ","
            << m << ","
            << randVal << ","
            << greedyVal << ","
            << bestLocalVal << ","
            << LOCAL_ITERS << ","
            << bestGraspVal << ","
            << GRASP_ITERS << ","
            << known[idx] << "\n";
        // << randTime      << ","
        // << greedyTime    << ","
        // << localTime     << ","
        // << graspTime     << "\n";
    }

    csv.close();
    cout << "Done! report.csv generated.\n";
    return 0;
}
