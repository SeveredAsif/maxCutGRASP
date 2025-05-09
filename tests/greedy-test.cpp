#include <iostream>
#include <cassert>
#include "../greedy_max_cut.hpp"

void test_two_node()
{
    // Graph: 0—(5)—1
    Graph g(2);
    g.addEdge(0, 1, 5);
    auto ans = greedyMaxCut(g, 2);
    auto &X = ans.first;
    auto &Y = ans.second;

    // The only sensible cut is {0} | {1}
    assert(X.size() == 1 && Y.size() == 1);
    assert((X.count(0) && Y.count(1)) || (X.count(1) && Y.count(0)));
}

void test_three_chain()
{
    // Graph: 0—(3)—1—(4)—2
    Graph g(3);
    g.addEdge(0, 1, 3);
    g.addEdge(1, 2, 4);
    auto ans = greedyMaxCut(g, 3);
    auto &X = ans.first;
    auto &Y = ans.second;

    // Heaviest edge is 1–2, so X={1},Y={2}; then 0 will go to X (wX(0)=0,wY(0)=3)
    assert(X.count(1) && Y.count(2));
    assert(Y.count(0));
}

void test_triangle()
{
    // Triangle: 0–1(2), 1–2(5), 0–2(1)
    Graph g(3);
    g.addEdge(0, 1, 2);
    g.addEdge(1, 2, 5);
    g.addEdge(0, 2, 1);
    auto ans = greedyMaxCut(g, 3);
    auto &X = ans.first;
    auto &Y = ans.second;

    // Heaviest edge 1–2 → X={1},Y={2}.
    // Now 0: wX(0)= w(0,2)=1 ; wY(0)= w(0,1)=2 → so 0→Y.
    // Final: X={1}, Y={0,2}
    assert(X.size() == 1 && X.count(1));
    assert(Y.size() == 2 && Y.count(0) && Y.count(2));
}

int main()
{
    test_two_node();
    test_three_chain();
    test_triangle();
    std::cout << "All tests passed!\n";
    return 0;
}
