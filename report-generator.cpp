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
#include "local-search.hpp"   // pair<vector<bool>,vector<bool>> local_search(vector<bool>&, ve\ctor<bool>&, Graph&)
#include "grasp.hpp"          // int grasp(int, Graph&)      // int calculate_cut_weight(...)
#include "semi_greedy.hpp"

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

    constexpr int RAND_ITERS = 1; // number of randomized runs
    // constexpr int LOCAL_ITERS = 5;  // multi-restart local-search
    constexpr int GRASP_ITERS = 25; // GRASP iterations
    int localiter = 0;

    ofstream csv("2105131FINAL.csv");
    csv << "Name,|V|,|E|,"
           "Rand1,Greedy1,SemiGreedy1,Local1_Average,Local1_Iter,Local1AvgLoop,GRASP_Iter,GRASP_Best,KnownBest\n";

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

        // semi-greedy
        auto tsemiStart = Clock::now();
        auto semigreedyVal = semiGreedy(g);
        auto tsemiEnd = Clock::now();
        auto semigreedyTime = chrono::duration_cast<ms>(tsemiEnd - tsemiStart).count();
        cout << id << ",SemiGreedy," << semigreedyTime << "ms\n";

        // 3) Simple Local (multi-restart, averaged over 5 runs)
        int RESTARTS = 5;
        int avgBestIter = 0;

        long long totalTime = 0;
        long long totalValue = 0;
        long long totalIter = 0;

        for (int run = 0; run < RESTARTS; ++run)
        {
            auto t4 = Clock::now();

            // 1) start from a greedy solution
            auto startP = rcl_algorithm(g,g.getSize(),0.5);

            // 2) improve using local search
            auto localP = local_search(startP.first,
                                       startP.second,
                                       g);

            // record how many local iterations it took
            int thisIter = localP.second;

            // evaluate the cut weight
            int thisVal = calculate_cut_weight(
                localP.first.first,
                localP.first.second,
                g.getEdge(),
                g.getWeights(),
                n);

            auto t5 = Clock::now();
            auto thisTime = chrono::duration_cast<ms>(t5 - t4).count();

            // accumulate
            totalTime += thisTime;
            totalValue += thisVal;
            totalIter += thisIter;

            // optional per-run logging
            cout << "Run " << (run + 1)
                 << ": LocalSearch = "
                 << thisVal << " (iter=" << thisIter << "), "
                 << thisTime << "ms\n";
        }

        // compute average
        double LocalavgValue = double(totalValue) / RESTARTS;
        avgBestIter = totalIter / RESTARTS;

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
            << semigreedyVal << ","
            << LocalavgValue << ","
            << RESTARTS << ","
            << avgBestIter << ","
            << GRASP_ITERS << ","
            << bestGraspVal << ","
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
