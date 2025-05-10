#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>  // for put_time
#include "grasp.hpp"
using namespace std;
using namespace chrono;

int main()
{
    ofstream outputFile("output50Iterations.txt");

    // Record the ultimate start time
    auto ultimate_start = system_clock::now();
    time_t start_time_t = system_clock::to_time_t(ultimate_start);
    tm *start_tm = localtime(&start_time_t);

    outputFile << "Start Time: " << put_time(start_tm, "%H:%M:%S") << endl;

    for (int graphNum = 1; graphNum <= 54; ++graphNum)
    {
        string fileName = "./input/g" + to_string(graphNum) + ".rud";
        freopen(fileName.c_str(), "r", stdin);

        int n, m;
        cin >> n >> m;

        Graph g(n);
        for (int i = 0; i < m; i++)
        {
            int u, v, w;
            cin >> u >> v >> w;
            g.addEdge(u, v, w);
        }

        // Record graph start time
        auto graph_start = high_resolution_clock::now();

        int ans = grasp(50, g);  // assuming grasp returns int (the solution value)

        // Record graph end time
        auto graph_end = high_resolution_clock::now();
        auto graph_duration = duration_cast<milliseconds>(graph_end - graph_start);

        outputFile << "Graph " << graphNum << ": "
                   << "Solution = " << ans << ", "
                   << "Time = " << graph_duration.count() / 1000.0 << " seconds" << endl;

        cout << "Processed Graph " << graphNum << endl;  // optional console log
    }

    // Record the ultimate end time
    auto ultimate_end = system_clock::now();
    time_t end_time_t = system_clock::to_time_t(ultimate_end);
    tm *end_tm = localtime(&end_time_t);

    outputFile << "End Time: " << put_time(end_tm, "%H:%M:%S") << endl;

    outputFile.close();
    return 0;
}
