#pragma once

#include "IGraph.h"

#include <cassert>
#include <unordered_set>


class SetGraph : public IGraph {
 public:
    SetGraph(std::size_t verticesCount);

    SetGraph(const IGraph& other);
    SetGraph& operator=(const IGraph& other);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const  override;
    std::vector<int> GetPrevVertices(int vertex) const override;

 private:
    std::vector<std::unordered_set<int>> hashTables;
};
