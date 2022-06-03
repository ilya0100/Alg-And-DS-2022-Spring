#include "MatrixGraph.h"


MatrixGraph::MatrixGraph(std::size_t verticesCount)
    : verticesMatrix(verticesCount, std::vector<int>(verticesCount, 0)) {}

MatrixGraph::MatrixGraph(const IGraph& other)
    : verticesMatrix(other.VerticesCount(), std::vector<int>(other.VerticesCount(), 0)) {

    for (int i = 0; i < other.VerticesCount(); ++i) {
        for (auto vertex : other.GetNextVertices(i)) {
            verticesMatrix[i][vertex] = 1;
        }
    }
}

MatrixGraph& MatrixGraph::operator=(const IGraph& other) {
    *this = MatrixGraph(other);
    return *this;
}

void MatrixGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < verticesMatrix.size());
    assert(to >= 0 && to < verticesMatrix.size());

    verticesMatrix[from][to] = 1;
}

int MatrixGraph::VerticesCount() const {
    return verticesMatrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < verticesMatrix.size());

    std::vector<int> nextVertices;
    for (std::size_t j = 0; j < verticesMatrix.size(); ++j) {
        if (verticesMatrix[vertex][j] == 1) {
            nextVertices.push_back(j);
        }
    }
    return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < verticesMatrix.size());

    std::vector<int> prevVertices;
    for (std::size_t i = 0; i < verticesMatrix.size(); ++i) {
        if (verticesMatrix[i][vertex] == 1) {
            prevVertices.push_back(i);
        }
    }
    return prevVertices;
}
