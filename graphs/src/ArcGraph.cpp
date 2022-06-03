#include "ArcGraph.h"


ArcGraph::ArcGraph(const IGraph& other): maxVertex(other.VerticesCount()) {
    for (int i = 0; i < maxVertex; ++i) {
        for (auto vertex : other.GetNextVertices(i)) {
            edges.push_back({i, vertex});
        }
    }
}

ArcGraph& ArcGraph::operator=(const IGraph& other) {
    *this = ArcGraph(other);
    return *this;
}

void ArcGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < maxVertex);
    assert(to >= 0 && to < maxVertex);

    edges.push_back({from, to});
}

int ArcGraph::VerticesCount() const {
    return maxVertex;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < maxVertex);

    std::vector<int> nextVertices;
    for (auto edge : edges) {
        if (edge.first == vertex) {
            nextVertices.push_back(edge.second);
        }
    }
    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < maxVertex);

    std::vector<int> prevVertices;
    for (auto edge : edges) {
        if (edge.second == vertex) {
            prevVertices.push_back(edge.first);
        }
    }
    return prevVertices;
}
