// Graph.h
#pragma once
#include <vector>
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>

struct Graph
{
    std::size_t numVertices;
    std::vector<std::vector<int>> adjacencyLists_outlinks;
    std::vector<std::vector<int>> adjacencyLists_inlinks;
};

void addEdge(Graph *graph, std::size_t source, std::size_t destination);

Graph *createGraph(std::size_t vertices);
