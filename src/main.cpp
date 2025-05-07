#include "graph.hpp"
#include "pageRank.hpp"
#include <iostream>
#include <iomanip> // for std::setprecision
#include <cstdlib> // for malloc/free

int main()
{
    // Number of nodes in the graph
    std::size_t N = 4;

    // 1. Create the graph
    Graph *graph = createGraph(N);
    if (!graph)
        return 1;

    // 2. Add edges – intentionally sparse, includes dangling nodes
    // A graph where some nodes have no outgoing edges
    addEdge(graph, 0, 1);
    addEdge(graph, 1, 2);
    addEdge(graph, 0, 2);
    addEdge(graph, 2, 3);

    // 3. Initialize PageRank values – uniform distribution
    float *ranks = (float *)malloc(N * sizeof(float));
    if (!ranks)
    {
        std::cerr << "Memory allocation failed for ranks\n";
        return 1;
    }
    for (std::size_t i = 0; i < N; ++i)
    {
        ranks[i] = 1.0f / N;
    }

    // 4. Run the PageRank algorithm
    PageRank(graph, 5, ranks);

    // 5. Print the final PageRank values
    std::cout << "Final PageRank values after 10 iterations:\n";
    std::cout << std::fixed << std::setprecision(6);
    for (std::size_t i = 0; i < N; ++i)
    {
        std::cout << "Node " << i << ": " << ranks[i] << '\n';
    }

    // 6. Free allocated memory
    free(ranks);
    free(graph); // only if created using malloc

    return 0;
}
