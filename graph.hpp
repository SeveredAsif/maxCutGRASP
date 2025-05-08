#include <list>
using namespace std;
class Graph
{
    int V;                  // No. of vertices in graph
    list<int> *adj;         // Pointer to an array containing adjacency lists
    list<int> *adj_weights; // Pointer to an array containing adjacency lists

public:
    Graph(int V)
    {
        this->V = V;
        adj = new list<int>[V];
        adj_weights = new list<int>[V];
    }
    void addEdge(int u, int v, int weight)
    {
        adj[u].push_back(v);              // Add v to u’s list.
        adj_weights[u].push_back(weight); // Add the weight of the path as well.

        adj[v].push_back(u);              // Add u to v’s list.
        adj_weights[v].push_back(weight); // Add the weight of the path as well.
    }
    void print() {
        for (int i = 0; i < V; i++) {
            cout << endl << i << ": ";
            
            auto it = adj[i].begin();             // iterator for adjacency list
            auto wt = adj_weights[i].begin();     // iterator for weight list
            
            while (it != adj[i].end() && wt != adj_weights[i].end()) {
                cout << "(" << *it << ", " << *wt << ") ";
                ++it;
                ++wt;
            }
        }
    } 
};