// #include <iostream>
// #include <vector>
// #include <set>
// #include <algorithm>
// #include <cassert>
// #include<set>
// #include<list>
// #include"../local-search.hpp"


// // Calculate the cut value for a given partition
// int calculate_cut_value(const std::set<int>& S, const std::set<int>& S_p, Graph& g) {
//     int cut_value = 0;
//     std::list<int> *adj = g.getEdge();
//     std::list<int> *adjWeights = g.getWeights();
    
//     for (auto v : S) {
//         auto itr = adj[v].begin();
//         auto ptr = adjWeights[v].begin();
        
//         while (itr != adj[v].end() && ptr != adjWeights[v].end()) {
//             if (S_p.find(*itr) != S_p.end()) {
//                 cut_value += *ptr;
//             }
//             ++itr;
//             ++ptr;
//         }
//     }
    
//     // Since each edge is counted twice (once from each side), divide by 2
//     return cut_value / 2;
// }

// void test_simple_graph() {
//     // Create a simple graph with 4 vertices
//     Graph g(4);
//     g.addEdge(0, 1, 1);
//     g.addEdge(0, 2, 2);
//     g.addEdge(0, 3, 3);
//     g.addEdge(1, 2, 4);
//     g.addEdge(2, 3, 5);
    
//     // Initial partition: S = {0,1}, S' = {2,3}
//     std::set<int> S = {0, 1};
//     std::set<int> S_p = {2, 3};
    
//     // Calculate initial cut value
//     int initial_cut = calculate_cut_value(S, S_p, g);
//     std::cout << "Initial cut value: " << initial_cut << std::endl;
    
//     // Apply local search
//     auto result = local_search(S, S_p, g);
//     std::set<int> final_S = result.first;
//     std::set<int> final_S_p = result.second;
    
//     // Calculate final cut value
//     int final_cut = calculate_cut_value(final_S, final_S_p, g);
//     std::cout << "Final cut value: " << final_cut << std::endl;
    
//     // Print the final partition
//     std::cout << "Final partition S: ";
//     for (auto v : final_S) std::cout << v << " ";
//     std::cout << std::endl;
    
//     std::cout << "Final partition S': ";
//     for (auto v : final_S_p) std::cout << v << " ";
//     std::cout << std::endl;
    
//     // Verify that the final cut is at least as good as the initial cut
//     assert(final_cut >= initial_cut);
// }

// void test_complete_graph() {
//     // Create a complete graph with 5 vertices and random weights
//     Graph g(5);
    
//     // Add edges with different weights
//     g.addEdge(0, 1, 3);
//     g.addEdge(0, 2, 1);
//     g.addEdge(0, 3, 4);
//     g.addEdge(0, 4, 2);
//     g.addEdge(1, 2, 5);
//     g.addEdge(1, 3, 2);
//     g.addEdge(1, 4, 3);
//     g.addEdge(2, 3, 6);
//     g.addEdge(2, 4, 4);
//     g.addEdge(3, 4, 1);
    
//     // Initial partition: S = {0,1,2}, S' = {3,4}
//     std::set<int> S = {0, 1, 2};
//     std::set<int> S_p = {3, 4};
    
//     // Calculate initial cut value
//     int initial_cut = calculate_cut_value(S, S_p, g);
//     std::cout << "Initial cut value: " << initial_cut << std::endl;
    
//     // Apply local search
//     auto result = local_search(S, S_p, g);
//     std::set<int> final_S = result.first;
//     std::set<int> final_S_p = result.second;
    
//     // Calculate final cut value
//     int final_cut = calculate_cut_value(final_S, final_S_p, g);
//     std::cout << "Final cut value: " << final_cut << std::endl;
    
//     // Print the final partition
//     std::cout << "Final partition S: ";
//     for (auto v : final_S) std::cout << v << " ";
//     std::cout << std::endl;
    
//     std::cout << "Final partition S': ";
//     for (auto v : final_S_p) std::cout << v << " ";
//     std::cout << std::endl;
    
//     // Verify that the final cut is at least as good as the initial cut
//     assert(final_cut >= initial_cut);
// }

// int main() {
//     std::cout << "Testing simple graph:" << std::endl;
//     test_simple_graph();
    
//     std::cout << "\nTesting complete graph:" << std::endl;
//     test_complete_graph();
    
//     std::cout << "\nAll tests passed!" << std::endl;
//     return 0;
// }
#include <iostream>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <numeric>
#include <fstream>
#include "../local-search.hpp"
#include<set>
#include<vector>

// Forward declarations
pair<set<int>, set<int>> local_search(set<int> &X, set<int> &Y, Graph &g);

// Function to calculate the cut weight between two partitions
int calculate_cut_weight(const set<int>& S, const set<int>& S_bar, list<int>* adj, list<int>* adjWeights, int V) {
    int total_weight = 0;
    
    for (int u : S) {
        auto itr = adj[u].begin();
        auto wtr = adjWeights[u].begin();
        
        while (itr != adj[u].end() && wtr != adjWeights[u].end()) {
            int v = *itr;
            int weight = *wtr;
            
            if (S_bar.find(v) != S_bar.end()) {
                total_weight += weight;
            }
            
            ++itr;
            ++wtr;
        }
    }
    
    return total_weight;
}

// Function to validate that S and S_bar form a valid partition
bool validate_partition(const set<int>& S, const set<int>& S_bar, int V) {
    // Check that S and S_bar are disjoint
    for (int v : S) {
        if (S_bar.find(v) != S_bar.end()) {
            std::cout << "Error: Vertex " << v << " is in both S and S_bar\n";
            return false;
        }
    }
    
    // Check that every vertex is in either S or S_bar
    for (int i = 0; i < V; i++) {
        if (S.find(i) == S.end() && S_bar.find(i) == S.end()) {
            std::cout << "Error: Vertex " << i << " is in neither S nor S_bar\n";
            return false;
        }
    }
    
    return true;
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

// Function to generate an initial random partition
pair<set<int>, set<int>> generate_random_partition(int V) {
    set<int> S, S_bar;
    
    for (int i = 0; i < V; i++) {
        if (rand() % 2 == 0) {
            S.insert(i);
        } else {
            S_bar.insert(i);
        }
    }
    
    // Handle edge case where all vertices end up in one set
    if (S.empty()) {
        int vertex = rand() % V;
        S.insert(vertex);
        S_bar.erase(vertex);
    } else if (S_bar.empty()) {
        int vertex = rand() % V;
        S_bar.insert(vertex);
        S.erase(vertex);
    }
    
    return {S, S_bar};
}

// Function to manually generate a partition for small graphs
pair<set<int>, set<int>> generate_unoptimal_partition(int V) {
    set<int> S, S_bar;
    
    // Put first half in S, second half in S_bar
    for (int i = 0; i < V; i++) {
        if (i < V/2) {
            S.insert(i);
        } else {
            S_bar.insert(i);
        }
    }
    
    return {S, S_bar};
}

// Test on a small graph with known optimal solution
void test_small_graph_with_known_solution() {
    std::cout << "\nTesting small graph with known optimal solution..." << std::endl;
    
    // Create a small graph where the optimal cut is known
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(0, 2, 10); // Heavy edge that should cross the cut
    g.addEdge(0, 3, 1);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 10); // Heavy edge that should cross the cut
    g.addEdge(2, 3, 1);
    
    // The optimal partition should have {0,3} in one set and {1,2} in another
    // Start with a non-optimal partition
    set<int> S = {0, 2};
    set<int> S_bar = {1, 3};
    
    int initial_cut = calculate_cut_weight(S, S_bar, g.getEdge(), g.getWeights(), 4);
    std::cout << "Initial cut weight: " << initial_cut << std::endl;
    
    // Run local search
    pair<set<int>, set<int>> result = local_search(S, S_bar, g);
    set<int> improved_S = result.first;
    set<int> improved_S_bar = result.second;
    
    std::cout << "Final partition S: ";
    for (int v : improved_S) std::cout << v << " ";
    std::cout << std::endl;
    
    std::cout << "Final partition S_bar: ";
    for (int v : improved_S_bar) std::cout << v << " ";
    std::cout << std::endl;
    
    int final_cut = calculate_cut_weight(improved_S, improved_S_bar, g.getEdge(), g.getWeights(), 4);
    std::cout << "Final cut weight: " << final_cut << std::endl;
    
    // The local search should find the optimal solution with cut weight 22
    assert(final_cut == 22);
    
    bool valid = validate_partition(improved_S, improved_S_bar, 4);
    assert(valid);
}

// Test on a larger graph
void test_random_larger_graph() {
    std::cout << "\nTesting random larger graph..." << std::endl;
    
    int V = 20;
    Graph g = generate_random_graph(V, 0.5, 1, 100);
    
    // Generate a random partition
    pair<set<int>, set<int>> initial = generate_random_partition(V);
    set<int> S = initial.first;
    set<int> S_bar = initial.second;
    
    int initial_cut = calculate_cut_weight(S, S_bar, g.getEdge(), g.getWeights(), V);
    std::cout << "Initial cut weight: " << initial_cut << std::endl;
    
    // Run local search
    pair<set<int>, set<int>> result = local_search(S, S_bar, g);
    set<int> improved_S = result.first;
    set<int> improved_S_bar = result.second;
    
    int final_cut = calculate_cut_weight(improved_S, improved_S_bar, g.getEdge(), g.getWeights(), V);
    std::cout << "Final cut weight: " << final_cut << std::endl;
    
    // The improved cut should be at least as good as the initial one
    assert(final_cut >= initial_cut);
    
    bool valid = validate_partition(improved_S, improved_S_bar, V);
    assert(valid);
}

// Test with multiple random initial partitions
void test_multiple_random_starts() {
    std::cout << "\nTesting multiple random starts..." << std::endl;
    
    int V = 15;
    Graph g = generate_complete_graph(V, 1, 100);
    
    int best_cut = 0;
    for (int run = 0; run < 5; run++) {
        // Generate a random partition
        pair<set<int>, set<int>> initial = generate_random_partition(V);
        set<int> S = initial.first;
        set<int> S_bar = initial.second;
        
        // Run local search
        pair<set<int>, set<int>> result = local_search(S, S_bar, g);
        set<int> improved_S = result.first;
        set<int> improved_S_bar = result.second;
        
        int cut = calculate_cut_weight(improved_S, improved_S_bar, g.getEdge(), g.getWeights(), V);
        std::cout << "Run " << run+1 << " cut weight: " << cut << std::endl;
        
        best_cut = std::max(best_cut, cut);
        
        bool valid = validate_partition(improved_S, improved_S_bar, V);
        assert(valid);
    }
    
    std::cout << "Best cut found: " << best_cut << std::endl;
}

// Test with an unoptimal starting solution
void test_with_unoptimal_start() {
    std::cout << "\nTesting with unoptimal starting solution..." << std::endl;
    
    int V = 30;
    Graph g = generate_random_graph(V, 0.3, 1, 100);
    
    // Generate a partition that is likely suboptimal
    pair<set<int>, set<int>> initial = generate_unoptimal_partition(V);
    set<int> S = initial.first;
    set<int> S_bar = initial.second;
    
    int initial_cut = calculate_cut_weight(S, S_bar, g.getEdge(), g.getWeights(), V);
    std::cout << "Initial cut weight: " << initial_cut << std::endl;
    
    // Run local search
    pair<set<int>, set<int>> result = local_search(S, S_bar, g);
    set<int> improved_S = result.first;
    set<int> improved_S_bar = result.second;
    
    int final_cut = calculate_cut_weight(improved_S, improved_S_bar, g.getEdge(), g.getWeights(), V);
    std::cout << "Final cut weight: " << final_cut << std::endl;
    
    // The improved cut should be at least as good as the initial one
    assert(final_cut >= initial_cut);
    
    bool valid = validate_partition(improved_S, improved_S_bar, V);
    assert(valid);
}

// Test with edge cases
void test_edge_cases() {
    std::cout << "\nTesting edge cases..." << std::endl;
    
    // Test with a graph with all equal weights
    {
        std::cout << "Equal weights test:" << std::endl;
        int V = 10;
        Graph g(V);
        
        for (int i = 0; i < V; i++) {
            for (int j = i+1; j < V; j++) {
                g.addEdge(i, j, 42);  // All weights are the same
            }
        }
        
        pair<set<int>, set<int>> initial = generate_random_partition(V);
        set<int> S = initial.first;
        set<int> S_bar = initial.second;
        
        int initial_cut = calculate_cut_weight(S, S_bar, g.getEdge(), g.getWeights(), V);
        std::cout << "  Initial cut weight: " << initial_cut << std::endl;
        
        pair<set<int>, set<int>> result = local_search(S, S_bar, g);
        set<int> improved_S = result.first;
        set<int> improved_S_bar = result.second;
        
        int final_cut = calculate_cut_weight(improved_S, improved_S_bar, g.getEdge(), g.getWeights(), V);
        std::cout << "  Final cut weight: " << final_cut << std::endl;
        
        bool valid = validate_partition(improved_S, improved_S_bar, V);
        assert(valid);
    }
    
    // Test with a very sparse graph
    {
        std::cout << "Sparse graph test:" << std::endl;
        int V = 20;
        Graph g = generate_random_graph(V, 0.1, 1, 100);  // Only 10% of edges
        
        pair<set<int>, set<int>> initial = generate_random_partition(V);
        set<int> S = initial.first;
        set<int> S_bar = initial.second;
        
        int initial_cut = calculate_cut_weight(S, S_bar, g.getEdge(), g.getWeights(), V);
        std::cout << "  Initial cut weight: " << initial_cut << std::endl;
        
        pair<set<int>, set<int>> result = local_search(S, S_bar, g);
        set<int> improved_S = result.first;
        set<int> improved_S_bar = result.second;
        
        int final_cut = calculate_cut_weight(improved_S, improved_S_bar, g.getEdge(), g.getWeights(), V);
        std::cout << "  Final cut weight: " << final_cut << std::endl;
        
        bool valid = validate_partition(improved_S, improved_S_bar, V);
        assert(valid);
    }
    
    // Test with a disconnected graph
    {
        std::cout << "Disconnected graph test:" << std::endl;
        int V = 20;
        Graph g(V);
        
        // Create two disconnected clusters
        for (int i = 0; i < V/2; i++) {
            for (int j = i+1; j < V/2; j++) {
                g.addEdge(i, j, rand() % 100 + 1);
            }
        }
        
        for (int i = V/2; i < V; i++) {
            for (int j = i+1; j < V; j++) {
                g.addEdge(i, j, rand() % 100 + 1);
            }
        }
        
        pair<set<int>, set<int>> initial = generate_random_partition(V);
        set<int> S = initial.first;
        set<int> S_bar = initial.second;
        
        int initial_cut = calculate_cut_weight(S, S_bar, g.getEdge(), g.getWeights(), V);
        std::cout << "  Initial cut weight: " << initial_cut << std::endl;
        
        pair<set<int>, set<int>> result = local_search(S, S_bar, g);
        set<int> improved_S = result.first;
        set<int> improved_S_bar = result.second;
        
        int final_cut = calculate_cut_weight(improved_S, improved_S_bar, g.getEdge(), g.getWeights(), V);
        std::cout << "  Final cut weight: " << final_cut << std::endl;
        
        bool valid = validate_partition(improved_S, improved_S_bar, V);
        assert(valid);
    }
}

// Function to test the local search algorithm's performance
void performance_test() {
    std::cout << "\nPerformance test..." << std::endl;
    
    std::vector<int> sizes = {10, 20, 50};
    
    for (int V : sizes) {
        std::cout << "Testing graph with " << V << " vertices" << std::endl;
        
        Graph g = generate_complete_graph(V, 1, 100);
        
        // Generate a random partition
        pair<set<int>, set<int>> initial = generate_random_partition(V);
        set<int> S = initial.first;
        set<int> S_bar = initial.second;
        
        int initial_cut = calculate_cut_weight(S, S_bar, g.getEdge(), g.getWeights(), V);
        std::cout << "  Initial cut weight: " << initial_cut << std::endl;
        
        // Measure execution time
        clock_t start = clock();
        
        // Run local search
        pair<set<int>, set<int>> result = local_search(S, S_bar, g);
        set<int> improved_S = result.first;
        set<int> improved_S_bar = result.second;
        
        clock_t end = clock();
        double elapsed = double(end - start) / CLOCKS_PER_SEC;
        
        int final_cut = calculate_cut_weight(improved_S, improved_S_bar, g.getEdge(), g.getWeights(), V);
        std::cout << "  Final cut weight: " << final_cut << std::endl;
        std::cout << "  Time elapsed: " << elapsed << " seconds" << std::endl;
        
        bool valid = validate_partition(improved_S, improved_S_bar, V);
        assert(valid);
    }
}

// Test for monotonicity - cut weight should never decrease during local search
void test_monotonicity() {
    std::cout << "\nTesting monotonicity..." << std::endl;
    
    int V = 15;
    Graph g = generate_random_graph(V, 0.5, 1, 100);
    
    // Generate a random partition
    pair<set<int>, set<int>> initial = generate_random_partition(V);
    set<int> S = initial.first;
    set<int> S_bar = initial.second;
    
    int initial_cut = calculate_cut_weight(S, S_bar, g.getEdge(), g.getWeights(), V);
    std::cout << "Initial cut weight: " << initial_cut << std::endl;
    
    // Run local search
    pair<set<int>, set<int>> result = local_search(S, S_bar, g);
    set<int> improved_S = result.first;
    set<int> improved_S_bar = result.second;
    
    int final_cut = calculate_cut_weight(improved_S, improved_S_bar, g.getEdge(), g.getWeights(), V);
    std::cout << "Final cut weight: " << final_cut << std::endl;
    
    // The final cut weight should be greater than or equal to the initial cut weight
    assert(final_cut >= initial_cut);
    
    bool valid = validate_partition(improved_S, improved_S_bar, V);
    assert(valid);
}

int main() {
    // Seed random number generator
    srand(time(NULL));
    
    std::cout << "Starting Local Search for MAX-CUT Tests" << std::endl;
    std::cout << "====================================" << std::endl;
    
    // Run the tests
    test_small_graph_with_known_solution();
    test_random_larger_graph();
    test_multiple_random_starts();
    test_with_unoptimal_start();
    test_edge_cases();
    performance_test();
    test_monotonicity();
    
    std::cout << "\nAll tests completed!" << std::endl;
    
    return 0;
}