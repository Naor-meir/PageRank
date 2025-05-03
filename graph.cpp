#include "graph.hpp"
// function to add an edge  to the graph 
// using adjacency list representation 
void addEdge(Graph *graph, std::size_t source, std::size_t destination) {
   graph->adjacencyLists_outlinks[source].push_back(destination);
    graph->adjacencyLists_inlinks[destination].push_back(source);
}
// initializes graph with given number of vertices 
Graph * createGraph(std::size_t vertices) {
    Graph *graph=(Graph*)calloc(1,sizeof(Graph));
    graph->numVertices = vertices;
    graph->adjacencyLists_outlinks.resize(vertices);
    graph->adjacencyLists_inlinks.resize(vertices);
    return graph;
}
