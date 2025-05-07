#include "pageRank.hpp"
#include <numeric>
#include <execution>

void PageRank(Graph *graph, int iterations, float *ranks)
{
    // number of vertices
    size_t N = graph->numVertices;
    float d = 0.15;                         // damping factor
    float constance = d / (float)N;         // constant value to add to each rank
    float zero_outlinkes_sum = 0.0;         // sum of ranks for nodes with zero outlinks intialized to 0.0
    std::vector<float> newRanks(N, 0.0f);   // Initialize new ranks vector
    ThreadPool pool(N);                     // Create a thread pool with the number of available threads
    std::vector<std::future<void>> futures; // Vector to hold futures for each thread
    futures.reserve(N);
    std::vector<float> Rank_zero_outlinkes(N, 0.0); // Initialize outlinkes vector
    for (int i = 0; i < iterations; i++)            // Loop for the number of iterations
    {
        futures.clear();                                                         // Clear the futures vector for each iteration
        std::fill(Rank_zero_outlinkes.begin(), Rank_zero_outlinkes.end(), 0.0f); // Reset outlinkes vector
        for (size_t v = 0; v < N; v++)
        {

            futures.emplace_back(
                pool.enqueue([v, &graph, &Rank_zero_outlinkes, &ranks, N]()
                             {                                                
                if (graph->adjacencyLists_outlinks[v].size() == 0) //check if the node has no outlinks
                Rank_zero_outlinkes[v] = ranks[v] / N; }));
        }

        for (auto &fut : futures)
        {
            fut.get(); // Wait for all threads to finish
        }
        zero_outlinkes_sum = std::reduce(std::execution::par, Rank_zero_outlinkes.begin(),
                                         Rank_zero_outlinkes.end(), 0.0f);
        // Calculate the sum of ranks for nodes with zero outlinks

        futures.clear(); // Clear the futures vector for the next iteration
        for (size_t u = 0; u < N; u++)
        {
            futures.emplace_back(
                pool.enqueue([u, &graph, &ranks, &newRanks, zero_outlinkes_sum, d, constance]()
                             {
                                 for (int v : graph->adjacencyLists_inlinks[u])
                                 // begin loop
                                 {
                                     std::size_t out_deg = graph->adjacencyLists_outlinks[v].size(); // Get the out-degree of vertex v

                                     float contribution = (ranks[v] / (float)out_deg) * (1 - d); // Calculate the contribution of vertex v to the rank of vertex u

                                     newRanks[u] += contribution; // Add the contribution to the new rank of vertex u
                                 }
                                 // end loop
                             }));
        }

        for (auto &fut : futures)
        {
            fut.get(); // Wait for all threads to finish
        }
        for (size_t i = 0; i < N; i++)
        {
            newRanks[i] += zero_outlinkes_sum * (1 - d) + constance; // Add the constant value to each rank
        }

        std::copy(newRanks.begin(), newRanks.end(), ranks);
        std::fill(newRanks.begin(), newRanks.end(), 0.0f);
    }
}
