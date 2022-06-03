#pragma once

#include "IGraph.h"

#include <cassert>


class ArcGraph : public IGraph {
 public:
    ArcGraph(std::size_t verticesCount): maxVertex(verticesCount) {}

    ArcGraph(const IGraph& other);
    ArcGraph& operator=(const IGraph& other);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const  override;
    std::vector<int> GetPrevVertices(int vertex) const override;

 private:
    int maxVertex;
    std::vector<std::pair<int, int>> edges;
};
