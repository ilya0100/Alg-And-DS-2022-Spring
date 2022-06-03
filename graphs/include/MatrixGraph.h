#pragma once

#include "IGraph.h"
#include <cassert>


class MatrixGraph : public IGraph {
 public:
    MatrixGraph(std::size_t verticesCount);

    MatrixGraph(const IGraph& other);
    MatrixGraph& operator=(const IGraph& other);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const  override;
    std::vector<int> GetPrevVertices(int vertex) const override;

 private:
    std::vector<std::vector<int>> verticesMatrix;
};
