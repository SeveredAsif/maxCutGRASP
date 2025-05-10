#include <iostream>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <numeric>
#include "../rcl_based.hpp"

// Function to validate that X and Y form a valid partition
bool validate_partition(const set<int>& X, const set<int>& Y, int V) {
    // Check that X and Y are disjoint
    for (int v : X) {
        if (Y.find(v) != Y.end()) {
            std::cout << "Error: Vertex " << v << " is in both X and Y\n";
            return false;
        }
    }
    
    // Check that every vertex is in either X or Y
    for (int i = 0; i < V; i++) {
        if (X.find(i) == X.end() && Y.find(i) == Y.end()) {
            std::cout << "Error: Vertex " << i << " is in neither X nor Y\n";
            return false;
        }
    }
    
    return true;
}

// Function to calculate the cut weight between X and Y
int calculate_cut_weight(const set<int>& X, const set<int>& Y, list<int>* adj, list<int>* adjWeights, int V) {
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

// Function to generate a random graph for testing
Graph generate_random_graph(int V, double edge_probability, int min_weight, int max_weight) {
    Graph g(V);
    
    for (int i = 0; i < V; i++) {
        for (int j = i+1; j < V; j++) {
            if ((double)rand() / RAND_MAX < edge_probability) {
                int weight = min_weight + rand() % (max_weight - min_weight + 1);
                g.addEdge(i, j, weight);
            }
        }
    }
    
    return g;
}

// Function to generate a complete graph for testing
Graph generate_complete_graph(int V, int min_weight, int max_weight) {
    Graph g(V);
    
    for (int i = 0; i < V; i++) {
        for (int j = i+1; j < V; j++) {
            int weight = min_weight + rand() % (max_weight - min_weight + 1);
            g.addEdge(i, j, weight);
        }
    }
    
    return g;
}

// Test case for a simple known graph
void test_simple_graph() {
    std::cout << "Testing simple graph..." << std::endl;
    
    // Create a simple graph with 4 vertices
    Graph g(4);
    g.addEdge(0, 1, 10);  // Heaviest edge
    g.addEdge(0, 2, 5);
    g.addEdge(0, 3, 3);
    g.addEdge(1, 2, 2);
    g.addEdge(1, 3, 4);
    g.addEdge(2, 3, 6);
    
    pair<set<int>, set<int>> result = rcl_algorithm(g, 4, 0.5);
    set<int> X = result.first;
    set<int> Y = result.second;
    
    std::cout << "X: ";
    for (int v : X) std::cout << v << " ";
    std::cout << std::endl;
    
    std::cout << "Y: ";
    for (int v : Y) std::cout << v << " ";
    std::cout << std::endl;
    
    bool valid = validate_partition(X, Y, 4);
    assert(valid);
    
    int cut_weight = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), 4);
    std::cout << "Cut weight: " << cut_weight << std::endl;
    
    // Since there's randomness in the algorithm, we can't assert a specific cut weight
    // But we can check that it's working and producing a valid partition
}

// Test with various alpha values
void test_alpha_values() {
    std::cout << "\nTesting different alpha values..." << std::endl;
    
    // Create a medium-sized graph
    int V = 20;
    Graph g = generate_random_graph(V, 0.5, 1, 100);
    
    // Test with different alpha values
    std::vector<double> alpha_values = {0.0, 0.25, 0.5, 0.75, 1.0};
    
    for (double alpha : alpha_values) {
        std::cout << "Alpha = " << alpha << std::endl;
        pair<set<int>, set<int>> result = rcl_algorithm(g, V, alpha);
        set<int> X = result.first;
        set<int> Y = result.second;
        
        bool valid = validate_partition(X, Y, V);
        assert(valid);
        
        int cut_weight = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), V);
        std::cout << "  Cut weight: " << cut_weight << std::endl;
        
        std::cout << "  X size: " << X.size() << ", Y size: " << Y.size() << std::endl;
    }
}

// Test with different graph sizes
void test_different_sizes() {
    std::cout << "\nTesting different graph sizes..." << std::endl;
    
    std::vector<int> sizes = {5, 10, 20, 50};
    double alpha = 0.5;
    
    for (int V : sizes) {
        std::cout << "Graph size = " << V << std::endl;
        Graph g = generate_random_graph(V, 0.5, 1, 100);
        
        pair<set<int>, set<int>> result = rcl_algorithm(g, V, alpha);
        set<int> X = result.first;
        set<int> Y = result.second;
        
        bool valid = validate_partition(X, Y, V);
        assert(valid);
        
        int cut_weight = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), V);
        std::cout << "  Cut weight: " << cut_weight << std::endl;
    }
}

// Test edge cases
void test_edge_cases() {
    std::cout << "\nTesting edge cases..." << std::endl;
    
    // Test with a complete graph
    {
        std::cout << "Complete graph test:" << std::endl;
        int V = 15;
        Graph g = generate_complete_graph(V, 1, 100);
        
        pair<set<int>, set<int>> result = rcl_algorithm(g, V, 0.5);
        set<int> X = result.first;
        set<int> Y = result.second;
        
        bool valid = validate_partition(X, Y, V);
        assert(valid);
        
        int cut_weight = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), V);
        std::cout << "  Cut weight: " << cut_weight << std::endl;
    }
    
    // Test with a graph with all equal weights
    {
        std::cout << "Equal weights test:" << std::endl;
        int V = 15;
        Graph g(V);
        
        for (int i = 0; i < V; i++) {
            for (int j = i+1; j < V; j++) {
                g.addEdge(i, j, 42);  // All weights are the same
            }
        }
        
        pair<set<int>, set<int>> result = rcl_algorithm(g, V, 0.5);
        set<int> X = result.first;
        set<int> Y = result.second;
        
        bool valid = validate_partition(X, Y, V);
        assert(valid);
        
        int cut_weight = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), V);
        std::cout << "  Cut weight: " << cut_weight << std::endl;
    }
    
    // Test with a very sparse graph
    {
        std::cout << "Sparse graph test:" << std::endl;
        int V = 20;
        Graph g = generate_random_graph(V, 0.1, 1, 100);  // Only 10% of edges
        
        pair<set<int>, set<int>> result = rcl_algorithm(g, V, 0.5);
        set<int> X = result.first;
        set<int> Y = result.second;
        
        bool valid = validate_partition(X, Y, V);
        assert(valid);
        
        int cut_weight = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), V);
        std::cout << "  Cut weight: " << cut_weight << std::endl;
    }
}

// Test for specific bugs in the algorithm
void test_specific_bugs() {
    std::cout << "\nTesting for specific bugs..." << std::endl;
    
    // Test for potential issues with U set maintenance
    {
        std::cout << "Testing U set maintenance:" << std::endl;
        int V = 10;
        Graph g = generate_complete_graph(V, 1, 10);
        
        pair<set<int>, set<int>> result = rcl_algorithm(g, V, 0.5);
        set<int> X = result.first;
        set<int> Y = result.second;
        
        bool valid = validate_partition(X, Y, V);
        assert(valid);
        
        // Verify X and Y sizes sum to V
        assert(X.size() + Y.size() == V);
        std::cout << "  X size: " << X.size() << ", Y size: " << Y.size() << std::endl;
    }
    
    // Test for potential issues with sigmaX and sigmaY calculations
    {
        std::cout << "Testing sigma calculations:" << std::endl;
        int V = 5; 
        Graph g(V);
        
        // Create a graph with asymmetric edge weights
        g.addEdge(0, 1, 10);
        g.addEdge(0, 2, 20);
        g.addEdge(0, 3, 5);
        g.addEdge(1, 2, 15);
        g.addEdge(1, 4, 30);  // High weight edge
        g.addEdge(2, 3, 8);
        g.addEdge(3, 4, 12);
        
        pair<set<int>, set<int>> result = rcl_algorithm(g, V, 0.5);
        set<int> X = result.first;
        set<int> Y = result.second;
        
        bool valid = validate_partition(X, Y, V);
        assert(valid);
        
        int cut_weight = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), V);
        std::cout << "  Cut weight: " << cut_weight << std::endl;
    }
}

int main() {
    // Seed random number generator
    srand(time(NULL));
    
    std::cout << "Starting RCL Algorithm Tests" << std::endl;
    std::cout << "===========================" << std::endl;
    
    // Run the tests
    test_simple_graph();
    test_alpha_values();
    test_different_sizes();
    test_edge_cases();
    test_specific_bugs();
    
    std::cout << "\nAll tests completed successfully!" << std::endl;
    
    return 0;
}