#include<iostream>
#include"graph.hpp"
using namespace std;

int main()
{
    int n,m;
    
    cin >> n >> m;
    Graph g(n);
    for(int i=0; i<m; i++){
        int u,v,w;
        cin >> u >> v >> w;
        g.addEdge(u,v,w);
    }  
    g.print();
}