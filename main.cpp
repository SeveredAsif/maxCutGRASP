#include <iostream>
#include <fstream>
#include <chrono>
#include "grasp.hpp"
using namespace std;
using namespace chrono;

int main()
{
    ofstream outputFile("output.txt");

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

        auto start = high_resolution_clock::now();

        int ans = grasp(5, g);  // assuming grasp returns int (the solution value)

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);

        // write to output file
        outputFile << "Graph " << graphNum << ": "
                   << "Solution = " << ans << ", "
                   << "Time = " << duration.count()/1000.0 << " s" << endl;

        cout << "Processed Graph " << graphNum << endl;  // optional console log
    }

    outputFile.close();
    return 0;
}


// #include<iostream>
// #include"grasp.hpp"
// using namespace std;

// int main()
// {
//     freopen("./input/g11.rud", "r", stdin);
//     int n,m;
//     cin >> n >> m;
//     cout<<n<<" "<<m<<endl;
//     Graph g(n);
//     for(int i=0; i<m; i++){
//         int u,v,w;
//         cin >> u >> v >> w;
//         g.addEdge(u,v,w);
//     }  
//     //g.print();
//     int ans = grasp(5,g);
//     cout<<ans<<endl;
// }