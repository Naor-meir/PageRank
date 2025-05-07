#pragma once 
#include <vector>
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "graph.hpp" // #include "graph.hpp"
#include "ThreadPool.hpp" // #include "ThreadPool.hpp"
void PageRank(Graph *graph, int iterations, float* ranks);
