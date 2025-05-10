// 4:37 - 5:09 
//5:09 - 5:19 - test and check
#include "local-search.hpp"
#include "rcl_based.hpp"

int calculate_cut_weight(const set<int>& X, const set<int>& Y, list<int>* adj, list<int>* adjWeights) {
    int total_weight = 0;
    
    for (int u : X) {
        auto itr = adj[u].begin();
        auto wtr = adjWeights[u].begin();
        
        while (itr != adj[u].end() && wtr != adjWeights[u].end()) {
            int v = *itr;
            int weight = *wtr;
            
            if (Y.find(v) != Y.end()) {
                total_weight += weight;
            }
            
            ++itr;
            ++wtr;
        }
    }
    
    return total_weight;
}

// Forward declarations
pair<set<int>, set<int>> local_search(set<int> &X, set<int> &Y, Graph &g);

int grasp(int iterations, Graph &g)
{
    int best_Wx = 0; 
    for (int i=0;i<iterations;i++)
    {
        
        pair<set<int>, set<int>> bestPartition;
        int size = g.getSize();
        pair<set<int>, set<int>> p = rcl_algorithm(g, size, 0.5);
        set<int> X = p.first;
        set<int> Y = p.second;
        p = local_search(X, Y, g);
        if(i==0) bestPartition = p;
        list<int>* adj = g.getEdge(); 
        list<int>* adjWeights = g.getWeights();
        int w_x = calculate_cut_weight(X,Y,adj,adjWeights); 
        if(w_x>best_Wx){
            best_Wx = w_x;
        }
    }
    return best_Wx;
}