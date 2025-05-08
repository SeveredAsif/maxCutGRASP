#include<list>
using namespace std;
class Graph {
    int V; // No. of vertices in graph
    list<int>* adj; // Pointer to an array containing adjacency lists
    list<int>* adj_weights; // Pointer to an array containing adjacency lists


Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
    adj_weights = new list<int>[V];
}
void Graph::addEdge(int u, int v, int weight)
{
    adj[u].push_back(v); // Add v to u’s list.
    adj_weights[u].push_back(weight); // Add the weight of the path as well.
}

};