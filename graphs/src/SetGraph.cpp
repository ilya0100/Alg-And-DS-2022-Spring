#include "SetGraph.h"


SetGraph::SetGraph(std::size_t verticesCount) {
    hashTables.resize(verticesCount);
}

SetGraph::SetGraph(const IGraph& other) {
    hashTables.resize(other.VerticesCount());
    for (int i = 0; i < other.VerticesCount(); ++i) {
        for (auto vertex : other.GetNextVertices(i)) {
            hashTables[i].insert(vertex);
        }
    }
}

SetGraph& SetGraph::operator=(const IGraph& other) {
    *this = SetGraph(other);
    return *this;
}

void SetGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < hashTables.size());
    assert(to >= 0 && to < hashTables.size());

    hashTables[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return hashTables.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < hashTables.size());
    std::vector<int> nextVertices;
    for (auto it : hashTables[vertex]) {
        nextVertices.push_back(it);
    }
    return nextVertices;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < hashTables.size());

    std::vector<int> prevVertices;
    for (std::size_t from = 0; from < hashTables.size(); ++from) {
        for (auto it : hashTables[from]) {
            if (it == vertex) {
                prevVertices.push_back(from);
                break;
            }
        }
    }
    return prevVertices;
}
