#include "ListGraph.h"


ListGraph::ListGraph(std::size_t verticesCount) {
    adjList.resize(verticesCount);
}

ListGraph::ListGraph(const IGraph& other) {
    adjList.resize(other.VerticesCount());
    for (int i = 0; i < other.VerticesCount(); ++i) {
        adjList[i] = other.GetNextVertices(i);
    }
}

ListGraph& ListGraph::operator=(const IGraph& other) {
    *this = ListGraph(other);
    return *this;
}

void ListGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjList.size());
    assert(to >= 0 && to < adjList.size());

    adjList[from].push_back(to);
}

int ListGraph::VerticesCount() const {
    return adjList.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjList.size());
    return adjList[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjList.size());

    std::vector<int> prevVertices;
    for (std::size_t from = 0; from < adjList.size(); ++from) {
        for (std::size_t i  = 0; i < adjList[from].size(); ++i) {
            if (adjList[from][i] == vertex) {
                prevVertices.push_back(from);
                break;
            }
        }
    }
    return prevVertices;
}
