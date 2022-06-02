#include "IGraph.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "ArcGraph.h"

#include <functional>
#include <queue>
#include <iostream>
#include <algorithm>
#include <memory>

#include <gtest/gtest.h>


void bfs(const IGraph& graph, int vertex, std::function<void(int)> visit) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> bfsQueue;
    bfsQueue.push(vertex);
    visited[vertex] = true;
    while (bfsQueue.size() > 0) {
        int current = bfsQueue.front();
        bfsQueue.pop();
        visit(current);
        std::vector<int> adjList = graph.GetNextVertices(current);
        for (size_t i = 0; i < adjList.size(); ++i) {
            if (!visited[adjList[i]]) {
                bfsQueue.push(adjList[i]);
                visited[adjList[i]] = true;
            }
        }
    }
}

void fillGraph(IGraph& graph) {
    graph.AddEdge(1, 3);
    graph.AddEdge(2, 1);
    graph.AddEdge(2, 3);
    graph.AddEdge(2, 4);
    graph.AddEdge(3, 4);
    graph.AddEdge(4, 5);
    graph.AddEdge(5, 3);
    graph.AddEdge(5, 4);
}


TEST(GraphsCreate, BFS) {
    std::shared_ptr<IGraph> listGraph = std::make_shared<ListGraph>(ListGraph(6));
    std::shared_ptr<IGraph> matrixGraph = std::make_shared<MatrixGraph>(MatrixGraph(6));
    std::shared_ptr<IGraph> arcGraph = std::make_shared<ArcGraph>(ArcGraph(6));

    fillGraph(*listGraph);
    fillGraph(*matrixGraph);
    fillGraph(*arcGraph);

    std::vector<int> listBFS;
    bfs(*listGraph, 1, [&listBFS](int v) {
        listBFS.push_back(v);
    });
    std::sort(listBFS.begin(), listBFS.end());

    std::vector<int> matrixBFS;
    bfs(*matrixGraph, 1, [&matrixBFS](int v) {
        matrixBFS.push_back(v);
    });
    std::sort(matrixBFS.begin(), matrixBFS.end());

    std::vector<int> arcBFS;
    bfs(*arcGraph, 1, [&arcBFS](int v) {
        arcBFS.push_back(v);
    });
    std::sort(arcBFS.begin(), arcBFS.end());

    std::vector<int> expectedResult = {1, 3, 4, 5};
    EXPECT_EQ(expectedResult, listBFS);
    EXPECT_EQ(expectedResult, matrixBFS);
    EXPECT_EQ(expectedResult, arcBFS);

    
}
