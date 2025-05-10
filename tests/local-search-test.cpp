#include <iostream>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <numeric>
#include "../local-search.hpp"

using namespace std;

// Function to calculate the cut weight between two partitions
int calculate_cut_weight(const vector<bool> &X, const vector<bool> &Y,
                         list<int> *adj, list<int> *adjWeights, int V)
{
    int total_weight = 0;

    for (int u = 0; u < V; u++)
    {
        if (!X[u])
            continue;

        auto itr = adj[u].begin();
        auto wtr = adjWeights[u].begin();

        while (itr != adj[u].end() && wtr != adjWeights[u].end())
        {
            int v = *itr;
            int weight = *wtr;

            if (Y[v])
            {
                total_weight += weight;
            }

            ++itr;
            ++wtr;
        }
    }

    return total_weight;
}

// Function to validate that X and Y form a valid partition
bool validate_partition(const vector<bool> &X, const vector<bool> &Y, int V)
{
    // Check that X and Y are disjoint
    for (int v = 0; v < V; v++)
    {
        if (X[v] && Y[v])
        {
            cout << "Error: Vertex " << v << " is in both X and Y\n";
            return false;
        }
    }

    // Check that every vertex is in either X or Y
    for (int v = 0; v < V; v++)
    {
        if (!X[v] && !Y[v])
        {
            cout << "Error: Vertex " << v << " is in neither X nor Y\n";
            return false;
        }
    }

    return true;
}

// Function to generate a random graph for testing
Graph generate_random_graph(int V, double edge_probability, int min_weight, int max_weight)
{
    Graph g(V);

    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            if ((double)rand() / RAND_MAX < edge_probability)
            {
                int weight = min_weight + rand() % (max_weight - min_weight + 1);
                g.addEdge(i, j, weight);
            }
        }
    }

    return g;
}

// Function to generate a complete graph for testing
Graph generate_complete_graph(int V, int min_weight, int max_weight)
{
    Graph g(V);

    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            int weight = min_weight + rand() % (max_weight - min_weight + 1);
            g.addEdge(i, j, weight);
        }
    }

    return g;
}

// Function to generate an initial random partition
pair<vector<bool>, vector<bool>> generate_random_partition(int V)
{
    vector<bool> X(V, false);
    vector<bool> Y(V, false);

    for (int i = 0; i < V; i++)
    {
        if (rand() % 2 == 0)
        {
            X[i] = true;
        }
        else
        {
            Y[i] = true;
        }
    }

    // Ensure at least one vertex in each set
    if (count(X.begin(), X.end(), true) == 0)
    {
        int v = rand() % V;
        X[v] = true;
        Y[v] = false;
    }
    if (count(Y.begin(), Y.end(), true) == 0)
    {
        int v = rand() % V;
        Y[v] = true;
        X[v] = false;
    }

    return {X, Y};
}

// Test on a small graph with known optimal solution
void test_small_graph_with_known_solution()
{
    cout << "\nTesting small graph with known optimal solution..." << endl;

    // Create a small graph where the optimal cut is known
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(0, 2, 10); // Heavy edge that should cross the cut
    g.addEdge(0, 3, 1);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 10); // Heavy edge that should cross the cut
    g.addEdge(2, 3, 1);

    // Start with a non-optimal partition
    vector<bool> X(4, false);
    vector<bool> Y(4, false);
    X[0] = X[2] = true;
    Y[1] = Y[3] = true;

    int initial_cut = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), 4);
    cout << "Initial cut weight: " << initial_cut << endl;

    // Run local search
    pair<vector<bool>, vector<bool>> p = local_search(X, Y, g);
    vector<bool> improved_X = p.first;
    vector<bool> improved_Y = p.second;

    cout << "Final partition X: ";
    for (int v = 0; v < 4; v++)
        if (improved_X[v])
            cout << v << " ";
    cout << endl;

    cout << "Final partition Y: ";
    for (int v = 0; v < 4; v++)
        if (improved_Y[v])
            cout << v << " ";
    cout << endl;

    int final_cut = calculate_cut_weight(improved_X, improved_Y, g.getEdge(), g.getWeights(), 4);
    cout << "Final cut weight: " << final_cut << endl;

    // The local search should find the optimal solution with cut weight 22
    assert(final_cut == 22);
    assert(validate_partition(improved_X, improved_Y, 4));
}

// Test on a larger graph
void test_random_larger_graph()
{
    cout << "\nTesting random larger graph..." << endl;

    int V = 20;
    Graph g = generate_random_graph(V, 0.5, 1, 100);

    // Generate a random partition
    pair<vector<bool>, vector<bool>> p = generate_random_partition(V);
    vector<bool> X = p.first;
    vector<bool> Y = p.second;

    int initial_cut = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), V);
    cout << "Initial cut weight: " << initial_cut << endl;

    // Run local search
    p = local_search(X, Y, g);
    vector<bool> improved_X = p.first;
    vector<bool> improved_Y = p.second;

    int final_cut = calculate_cut_weight(improved_X, improved_Y, g.getEdge(), g.getWeights(), V);
    cout << "Final cut weight: " << final_cut << endl;

    // The improved cut should be at least as good as the initial one
    assert(final_cut >= initial_cut);
    assert(validate_partition(improved_X, improved_Y, V));
}

// Test with multiple random initial partitions
void test_multiple_random_starts()
{
    cout << "\nTesting multiple random starts..." << endl;

    int V = 15;
    Graph g = generate_complete_graph(V, 1, 100);

    int best_cut = 0;
    for (int run = 0; run < 5; run++)
    {
        // Generate a random partition
        pair<vector<bool>, vector<bool>> p = generate_random_partition(V);
        vector<bool> X = p.first;
        vector<bool> Y = p.second;

        // Run local search
        p = local_search(X, Y, g);
        vector<bool> improved_X = p.first;
        vector<bool> improved_Y = p.second;

        int cut = calculate_cut_weight(improved_X, improved_Y, g.getEdge(), g.getWeights(), V);
        cout << "Run " << run + 1 << " cut weight: " << cut << endl;

        best_cut = max(best_cut, cut);
        assert(validate_partition(improved_X, improved_Y, V));
    }

    cout << "Best cut found: " << best_cut << endl;
}

// Test edge cases
void test_edge_cases()
{
    cout << "\nTesting edge cases..." << endl;

    // Test with all equal weights
    {
        cout << "Equal weights test:" << endl;
        int V = 10;
        Graph g(V);

        for (int i = 0; i < V; i++)
        {
            for (int j = i + 1; j < V; j++)
            {
                g.addEdge(i, j, 42);
            }
        }

        pair<vector<bool>, vector<bool>> p = generate_random_partition(V);
        vector<bool> X = p.first;
        vector<bool> Y = p.second;
        int initial_cut = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), V);
        cout << "  Initial cut weight: " << initial_cut << endl;

        p = local_search(X, Y, g);
        vector<bool> improved_X = p.first;
        vector<bool> improved_Y = p.second;
        int final_cut = calculate_cut_weight(improved_X, improved_Y, g.getEdge(), g.getWeights(), V);
        cout << "  Final cut weight: " << final_cut << endl;

        assert(validate_partition(improved_X, improved_Y, V));
    }

    // Test with a sparse graph
    {
        cout << "Sparse graph test:" << endl;
        int V = 20;
        Graph g = generate_random_graph(V, 0.1, 1, 100);

        pair<vector<bool>, vector<bool>> p = generate_random_partition(V);
        vector<bool> X = p.first;
        vector<bool> Y = p.second;
        int initial_cut = calculate_cut_weight(X, Y, g.getEdge(), g.getWeights(), V);
        cout << "  Initial cut weight: " << initial_cut << endl;

        p = local_search(X, Y, g);
        vector<bool> improved_X = p.first;
        vector<bool> improved_Y = p.second;
        int final_cut = calculate_cut_weight(improved_X, improved_Y, g.getEdge(), g.getWeights(), V);
        cout << "  Final cut weight: " << final_cut << endl;

        assert(validate_partition(improved_X, improved_Y, V));
    }
}

int main()
{
    srand(time(NULL));

    cout << "Starting Local Search Tests" << endl;
    cout << "==========================" << endl;

    test_small_graph_with_known_solution();
    test_random_larger_graph();
    test_multiple_random_starts();
    test_edge_cases();

    cout << "\nAll tests passed!" << endl;
    return 0;
}