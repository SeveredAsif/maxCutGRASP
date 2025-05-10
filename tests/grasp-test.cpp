#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <random>
#include <algorithm>
#include <cassert>
#include <iomanip>
#include "../grasp.hpp"

// Utility function to generate a random graph
Graph generate_random_graph(int vertices, double density, int min_weight, int max_weight) {
    Graph g(vertices);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> weight_dist(min_weight, max_weight);
    std::uniform_real_distribution<> edge_dist(0.0, 1.0);
    
    for (int i = 0; i < vertices; ++i) {
        for (int j = i + 1; j < vertices; ++j) {
            if (edge_dist(gen) < density) {
                int weight = weight_dist(gen);
                g.addEdge(i, j, weight);
            }
        }
    }
    
    return g;
}

// Utility function to convert set to vector<bool>
std::vector<bool> set_to_vectorbool(const std::set<int>& s, int size) {
    std::vector<bool> result(size, false);
    for (int elem : s) {
        result[elem] = true;
    }
    return result;
}

// Utility function to convert vector<bool> to set for printing
std::set<int> vectorbool_to_set(const std::vector<bool>& v) {
    std::set<int> result;
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i]) {
            result.insert(i);
        }
    }
    return result;
}

// Test on a small known graph
void test_small_known_graph() {
    std::cout << "\n=== Testing on small known graph ===\n" << std::endl;
    
    // Create a small graph with known optimal solution
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(0, 2, 10);
    g.addEdge(0, 3, 1);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 10);
    g.addEdge(2, 3, 1);
    
    // Initial partition
    std::vector<bool> X(4, false);
    std::vector<bool> Y(4, false);
    X[0] = true;
    X[1] = true;
    Y[2] = true;
    Y[3] = true;
    
    // Calculate initial cut weight
    int initial_weight = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), g.getSize());
    std::cout << "Initial partition: S = {0, 1}, S' = {2, 3}" << std::endl;
    std::cout << "Initial cut weight: " << initial_weight << std::endl;
    
    // Apply local search
    auto start_time = std::chrono::high_resolution_clock::now();
    std::pair<std::vector<bool>, std::vector<bool>> result = local_search(X, Y, g);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    std::vector<bool> final_X = result.first;
    std::vector<bool> final_Y = result.second;
    
    int final_weight = calculate_cut_weight(final_X, final_Y, g.getEdge(), g.getWeights(), g.getSize());
    
    // Convert to sets for printing
    std::set<int> final_X_set = vectorbool_to_set(final_X);
    std::set<int> final_Y_set = vectorbool_to_set(final_Y);
    
    std::cout << "Final partition: S = {";
    for (int v : final_X_set) std::cout << " " << v;
    std::cout << " }, S' = {";
    for (int v : final_Y_set) std::cout << " " << v;
    std::cout << " }" << std::endl;
    
    std::cout << "Final cut weight: " << final_weight << std::endl;
    
    std::chrono::duration<double, std::milli> duration = end_time - start_time;
    std::cout << "Local search execution time: " << duration.count() << " ms" << std::endl;
    
    // Verify that local search doesn't decrease the cut weight
    assert(final_weight >= initial_weight);
    std::cout << "Test passed: Local search does not decrease cut weight" << std::endl;
}

// Test with different RCL alpha values
void test_rcl_alpha_variations() {
    std::cout << "\n=== Testing RCL with different alpha values ===\n" << std::endl;
    
    // Generate a medium-sized random graph
    int vertices = 20;
    Graph g = generate_random_graph(vertices, 0.5, 1, 10);
    
    std::vector<double> alpha_values = {0.1, 0.3, 0.5, 0.7, 0.9};
    
    std::cout << std::left << std::setw(10) << "Alpha" 
              << std::setw(15) << "Initial Cut" 
              << std::setw(15) << "Final Cut" 
              << std::setw(20) << "Construction Time" 
              << std::setw(20) << "Local Search Time" << std::endl;
    
    for (double alpha : alpha_values) {
        // Construction phase timing
        auto start_construction = std::chrono::high_resolution_clock::now();
        std::pair<std::vector<bool>, std::vector<bool>> p = rcl_algorithm(g, vertices, alpha);
        auto end_construction = std::chrono::high_resolution_clock::now();
        
        std::vector<bool> X = p.first;
        std::vector<bool> Y = p.second;
        
        int initial_cut = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), vertices);
        
        // Local search phase timing
        auto start_local_search = std::chrono::high_resolution_clock::now();
        p = local_search(X, Y, g);
        auto end_local_search = std::chrono::high_resolution_clock::now();
        
        X = p.first;
        Y = p.second;
        
        int final_cut = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), vertices);
        
        std::chrono::duration<double, std::milli> construction_time = end_construction - start_construction;
        std::chrono::duration<double, std::milli> local_search_time = end_local_search - start_local_search;
        
        std::cout << std::left << std::setw(10) << alpha 
                  << std::setw(15) << initial_cut 
                  << std::setw(15) << final_cut 
                  << std::setw(20) << construction_time.count() 
                  << std::setw(20) << local_search_time.count() << std::endl;
        
        // Verify that local search doesn't decrease the cut weight
        assert(final_cut >= initial_cut);
    }
}

// Test GRASP with different iterations
void test_grasp_iterations() {
    std::cout << "\n=== Testing GRASP with different iterations ===\n" << std::endl;
    
    // Generate a medium-sized random graph
    int vertices = 30;
    Graph g = generate_random_graph(vertices, 0.4, 1, 10);
    
    std::vector<int> iterations = {1, 5, 10, 20};
    
    std::cout << std::left << std::setw(15) << "Iterations" 
              << std::setw(15) << "Best Cut" 
              << std::setw(20) << "Execution Time (ms)" 
              << std::setw(20) << "Avg Time per Iter" << std::endl;
    
    for (int iters : iterations) {
        auto start_time = std::chrono::high_resolution_clock::now();
        int best_cut = grasp(iters, g);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> duration = end_time - start_time;
        double avg_time_per_iter = duration.count() / iters;
        
        std::cout << std::left << std::setw(15) << iters 
                  << std::setw(15) << best_cut 
                  << std::setw(20) << duration.count() 
                  << std::setw(20) << avg_time_per_iter << std::endl;
    }
}

// Test on large graphs
void test_large_graphs() {
    std::cout << "\n=== Testing on large graphs ===\n" << std::endl;
    
    std::vector<int> vertices_sizes = {50, 100, 200};
    double density = 0.3; // 30% of possible edges
    
    std::cout << std::left << std::setw(15) << "Vertices" 
              << std::setw(15) << "GRASP Cut" 
              << std::setw(20) << "GRASP Time (ms)" 
              << std::setw(25) << "Local Search Time (ms)" << std::endl;
    
    for (int size : vertices_sizes) {
        Graph g = generate_random_graph(size, density, 1, 10);
        
        // Test single iteration of construction + local search
        auto start_construction = std::chrono::high_resolution_clock::now();
        std::pair<std::vector<bool>, std::vector<bool>> p = rcl_algorithm(g, size, 0.5);
        auto end_construction = std::chrono::high_resolution_clock::now();
        
        std::vector<bool> X = p.first;
        std::vector<bool> Y = p.second;
        
        auto start_local_search = std::chrono::high_resolution_clock::now();
        p = local_search(X, Y, g);
        auto end_local_search = std::chrono::high_resolution_clock::now();
        
        X = p.first;
        Y = p.second;
        
        int local_search_cut = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), size);
        std::chrono::duration<double, std::milli> local_search_time = end_local_search - start_local_search;
        
        // Test GRASP with 3 iterations
        auto start_grasp = std::chrono::high_resolution_clock::now();
        int grasp_cut = grasp(3, g);
        auto end_grasp = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> grasp_time = end_grasp - start_grasp;
        
        std::cout << std::left << std::setw(15) << size 
                  << std::setw(15) << grasp_cut 
                  << std::setw(20) << grasp_time.count() 
                  << std::setw(25) << local_search_time.count() << std::endl;
    }
}

// Compare with a simple random partition
void compare_with_random() {
    std::cout << "\n=== Comparing with random partitioning ===\n" << std::endl;
    
    int vertices = 50;
    Graph g = generate_random_graph(vertices, 0.5, 1, 10);
    
    // Create a random partition
    std::vector<bool> X(vertices, false);
    std::vector<bool> Y(vertices, false);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    
    for (int i = 0; i < vertices; ++i) {
        if (dis(gen) == 0) {
            X[i] = true;
        } else {
            Y[i] = true;
        }
    }
    
    int random_cut = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), vertices);
    
    // Apply local search
    auto start_time = std::chrono::high_resolution_clock::now();
    std::pair<std::vector<bool>, std::vector<bool>> result = local_search(X, Y, g);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    std::vector<bool> final_X = result.first;
    std::vector<bool> final_Y = result.second;
    
    int improved_cut = calculate_cut_weight(final_X, final_Y, g.getEdge(), g.getWeights(), vertices);
    
    std::chrono::duration<double, std::milli> duration = end_time - start_time;
    
    // Run GRASP with 5 iterations
    auto start_grasp = std::chrono::high_resolution_clock::now();
    int grasp_cut = grasp(5, g);
    auto end_grasp = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double, std::milli> grasp_time = end_grasp - start_grasp;
    
    std::cout << "Random partition cut: " << random_cut << std::endl;
    std::cout << "After local search: " << improved_cut << " (improvement: " 
              << (improved_cut - random_cut) << ", " 
              << (100.0 * (improved_cut - random_cut) / random_cut) << "%)" << std::endl;
    std::cout << "Local search time: " << duration.count() << " ms" << std::endl;
    std::cout << "GRASP (5 iterations) cut: " << grasp_cut << std::endl;
    std::cout << "GRASP time: " << grasp_time.count() << " ms" << std::endl;
    
    assert(improved_cut >= random_cut);
}

int main() {
    std::cout << "=== Maximum Cut Problem Test Suite ===\n" << std::endl;
    
    // Run all tests
    test_small_known_graph();
    test_rcl_alpha_variations();
    test_grasp_iterations();
    test_large_graphs();
    compare_with_random();
    
    std::cout << "\nAll tests passed successfully!" << std::endl;
    return 0;
}