# Makefile לפרויקט PageRank עם ThreadPool

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -pthread

SRC = main.cpp graph.cpp pageRank.cpp ThreadPool.cpp
HDR = graph.hpp pageRank.hpp ThreadPool.hpp
EXE = pagerank

all: $(EXE)

$(EXE): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(SRC)

run: $(EXE)
	./$(EXE)

clean:
	rm -f $(EXE)
