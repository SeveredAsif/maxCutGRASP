#include <iostream>
#include <cassert>
#include "../randomized.hpp"


// Helper: run randomized many times and assert that the average cut weight is 
// between the known min and max possible cut.
void test_two_node() {
    // Graph: 0—(5)—1; best cut = 5, worst = 0
    Graph g(2);
    g.addEdge(0, 1, 5);

    double avg = randomized(g, 1000);
    // Must be between 0 and 5
    assert(avg >= 0.0 && avg <= 5.0);
}

void test_three_chain() {
    // 0—3—1—4—2 ; best cut = 7 (separate {0,2} and {1}), worst = 3
    Graph g(3);
    g.addEdge(0, 1, 3);
    g.addEdge(1, 2, 4);

    double avg = randomized(g, 10000);
    // Expect average roughly around (best + worst)/2 = (7 + 3)/2 = 5
    // but allow some slack
    assert(avg >= 2.0 && avg <= 8.0);
}

void test_triangle_random_average() {
    // Triangle with all weights 1: expected average cut weight = 1.5
    Graph g(3);
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, 1);
    g.addEdge(0, 2, 1);

    double avg = randomized(g, 10000);
    // Expect avg ≈ 1.5, allow a small tolerance for randomness
    assert(fabs(avg - 1.5) < 0.1);
}


int main() {
    test_two_node();
    test_three_chain();
    test_triangle_random_average();
    std::cout << "All randomized tests passed!\n";
    return 0;
}

// #include <cassert>
// #include <cmath>
// #include<iostream>
// #include "../randomized.hpp"
// using namespace std;

// int main() {
//     srand(time(0));                   // seed once here
//     Graph g(2);
//     g.addEdge(0, 1, 5);

//     double avg1 = randomized(g, 1000);
//     assert(avg1 >= 0.0 && avg1 <= 5.0);

//     // On a 2-node graph, each random cut is either
//     // {0}|{1} (weight=5) or {1}|{0} (weight=5) or
//     // both in the same side (weight=0).
//     // Expected average ≈ (5 + 5 + 0 + 0) / 4 = 2.5
//     assert(std::fabs(avg1 - 2.5) < 1.0); 

//     cout << "Basic randomized test passed!\n";
//     return 0;
// }
