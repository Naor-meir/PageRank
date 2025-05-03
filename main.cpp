#include "graph.hpp"
#include "pageRank.hpp"
#include <iostream>
#include <iomanip> // std::setprecision
#include <cstdlib> // malloc/free

int main()
{
    // מספר קודקודים
    std::size_t N = 4;

    // 1. יצירת גרף
    Graph *graph = createGraph(N);
    if (!graph)
        return 1;

    // 2. הוספת קשתות – intentionally sparse, includes dangling nodes
    // גרף עם חלק מהצמתים שאין להם קשתות יוצאות
    addEdge(graph, 0, 1);
    addEdge(graph, 1, 2);
    addEdge(graph, 0, 2);
    addEdge(graph, 2, 3);

    // 3. אתחול ranks – אחיד
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

    // 4. הרצת PageRank
    PageRank(graph, 5, ranks);

    // 5. הדפסת תוצאה
    std::cout << "Final PageRank values after 10 iterations:\n";
    std::cout << std::fixed << std::setprecision(6);
    for (std::size_t i = 0; i < N; ++i)
    {
        std::cout << "Node " << i << ": " << ranks[i] << '\n';
    }

    // 6. ניקוי זיכרון
    free(ranks);
    free(graph); // אם אתה ב־malloc

    return 0;
}
