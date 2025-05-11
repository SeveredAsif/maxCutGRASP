#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <limits>
#include <chrono>
#include <random>
#include <ctime>
#include <iomanip>
#include "graph.hpp"
#include "randomized.hpp"     // double randomized(Graph&, int)
#include "greedy_max_cut.hpp" // pair<vector<bool>,vector<bool>> greedyMaxCut(Graph&, int)
#include "semi_greedy.hpp"    // int semiGreedy(Graph&)
#include "local-search.hpp"   // pair<vector<bool>,vector<bool>> local_search(vector<bool>&, vector<bool>&, Graph&)
#include "grasp.hpp"          // int grasp(int, Graph&)

using namespace std;
using Clock = chrono::high_resolution_clock;
using ms = chrono::milliseconds;

int calculate_cut_weight(const vector<bool>& X, const vector<bool>& Y, 
                        list<int>* adj, list<int>* adjWeights, int V);

int main() {
    constexpr int TOTAL_GRAPHS = 54;
    constexpr int SAMPLE_SIZE = 10;
    constexpr int RAND_ITERS = 1;
    constexpr int RESTARTS = 5;
    constexpr int GRASP_ITERS = 25;

    // generate a random sample of graph IDs from 1..TOTAL_GRAPHS
    vector<int> ids(TOTAL_GRAPHS);
    iota(ids.begin(), ids.end(), 1);
    random_device rd;
    mt19937 gen(rd());
    shuffle(ids.begin(), ids.end(), gen);
    ids.resize(SAMPLE_SIZE);

    // open CSV and write header with timestamps
    auto t_start = Clock::now();
    ofstream csv("results2.csv");
    csv << "RunTimestamp,Name,|V|,|E|,Rand,Greedy,SemiGreedy,LocalAvg,GRASP,RandTime,GreedyTime,SemiTime,LocalTimeAvg,GRASPTime" << '\n';

    for(int idx = 0; idx < SAMPLE_SIZE; ++idx) {
        int graphId = ids[idx];
        string name = to_string(graphId);
        // timestamp for this run
        time_t runTime = chrono::system_clock::to_time_t(chrono::system_clock::now());

        // --- load graph ---
        ifstream fin("./input/g" + name + ".rud");
        cout<<"graph: "<<name<<endl;
        if (!fin) {
            cerr << "Error opening file: " << name << ".rud" << endl;
            continue;
        }
        
        int n, m;
        fin >> n >> m;
        Graph g(n);
        for(int i = 0; i < m; ++i) {
            int u, v, w;
            fin >> u >> v >> w;
            g.addEdge(u, v, w);
        }
        fin.close();

        // 1) Randomized
        auto t0 = Clock::now();
        double randVal = randomized(g, RAND_ITERS);
        auto t1 = Clock::now();
        auto randTime = chrono::duration_cast<ms>(t1 - t0).count();
        cout<<"rand: "<<randTime<<endl;

        // 2) Greedy
        auto t2 = Clock::now();
        auto greedyP = greedyMaxCut(g, n);
        int greedyVal = calculate_cut_weight(
            greedyP.first, greedyP.second,
            g.getEdge(), g.getWeights(), n);
        auto t3 = Clock::now();
        auto greedyTime = chrono::duration_cast<ms>(t3 - t2).count();
        cout<<"greedyTime: "<<greedyTime<<endl;

        // 3) Semi-Greedy
        auto tSemi0 = Clock::now();
        int semiVal = semiGreedy(g);
        auto tSemi1 = Clock::now();
        auto semiTime = chrono::duration_cast<ms>(tSemi1 - tSemi0).count();
        cout<<"semiTime: "<<semiTime<<endl;

        // 4) Local Search (multi-restart, averaged over RESTARTS runs)
        long long totalLocalTime = 0;
        long long totalLocalVal = 0;
        long long totalLocalIter = 0;
        for(int run = 0; run < RESTARTS; ++run) {
            auto t4 = Clock::now();

            // start from RCL-based semi-greedy
            auto startP = rcl_algorithm(g, n, 0.5);
            auto localP = local_search(startP.first, startP.second, g);
            int thisVal = calculate_cut_weight(
                localP.first.first,
                localP.first.second,
                g.getEdge(), g.getWeights(), n);
            auto t5 = Clock::now();
            auto thisTime = chrono::duration_cast<ms>(t5 - t4).count();

            totalLocalTime += thisTime;
            totalLocalVal += thisVal;
        }
        double localAvgVal = double(totalLocalVal) / RESTARTS;
        double localAvgTime = double(totalLocalTime) / RESTARTS;
        cout<<"totalLocalTime: "<<totalLocalTime<<endl;

        // 5) GRASP
        auto t6 = Clock::now();
        int graspVal = grasp(GRASP_ITERS, g);
        auto t7 = Clock::now();
        auto graspTime = chrono::duration_cast<ms>(t7 - t6).count();
        cout<<"graspTime: "<<graspTime<<endl;

        // write CSV line
        csv << put_time(localtime(&runTime), "%F %T") << ','
            << name << ','
            << n << ',' << m << ','
            << randVal << ','
            << greedyVal << ','
            << semiVal << ','
            << localAvgVal << ','
            << graspVal << ','
            << randTime << ','
            << greedyTime << ','
            << semiTime << ','
            << localAvgTime << ','
            << graspTime << '\n';
    }
    csv.close();
    auto t_end = Clock::now();
    auto totalMs = chrono::duration_cast<ms>(t_end - t_start).count();
    cout << "Report generated in " << totalMs << "ms.\n";
    return 0;
}

