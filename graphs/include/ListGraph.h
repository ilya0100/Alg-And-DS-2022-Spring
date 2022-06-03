#pragma once

#include "IGraph.h"
#include <cassert>


class ListGraph : public IGraph {
 public:
    ListGraph(std::size_t verticesCount);

    ListGraph(const IGraph& other);
    ListGraph& operator=(const IGraph& other);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const  override;
    std::vector<int> GetPrevVertices(int vertex) const override;

 private:
    std::vector<std::vector<int>> adjList;
};
