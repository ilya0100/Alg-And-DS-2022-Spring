#include "IGraph.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "ArcGraph.h"
#include "SetGraph.h"

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


TEST(Graphs, BFS) {
    std::shared_ptr<IGraph> listGraph = std::make_shared<ListGraph>(ListGraph(6));
    std::shared_ptr<IGraph> matrixGraph = std::make_shared<MatrixGraph>(MatrixGraph(6));
    std::shared_ptr<IGraph> arcGraph = std::make_shared<ArcGraph>(ArcGraph(6));
    std::shared_ptr<IGraph> setGraph = std::make_shared<SetGraph>(SetGraph(6));

    fillGraph(*listGraph);
    fillGraph(*matrixGraph);
    fillGraph(*arcGraph);
    fillGraph(*setGraph);

    int vertex = 2;
    std::vector<int> listBFS;
    bfs(*listGraph, vertex, [&listBFS](int v) {
        listBFS.push_back(v);
    });
    std::sort(listBFS.begin(), listBFS.end());

    std::vector<int> matrixBFS;
    bfs(*matrixGraph, vertex, [&matrixBFS](int v) {
        matrixBFS.push_back(v);
    });
    std::sort(matrixBFS.begin(), matrixBFS.end());

    std::vector<int> arcBFS;
    bfs(*arcGraph, vertex, [&arcBFS](int v) {
        arcBFS.push_back(v);
    });
    std::sort(arcBFS.begin(), arcBFS.end());

    std::vector<int> setBFS;
    bfs(*setGraph, vertex, [&setBFS](int v) {
        setBFS.push_back(v);
    });
    std::sort(setBFS.begin(), setBFS.end());

    std::vector<int> expectedResult = {1, 2, 3, 4, 5};
    EXPECT_EQ(expectedResult, listBFS);
    EXPECT_EQ(expectedResult, matrixBFS);
    EXPECT_EQ(expectedResult, arcBFS);
    EXPECT_EQ(expectedResult, setBFS);
}

TEST(Graphs, Convertation) {
    std::shared_ptr<IGraph> listGraph = std::make_shared<ListGraph>(ListGraph(5));

    listGraph->AddEdge(1, 2);
    listGraph->AddEdge(3, 2);
    listGraph->AddEdge(2, 1);
    listGraph->AddEdge(4, 2);
    listGraph->AddEdge(4, 3);
    listGraph->AddEdge(0, 1);
    listGraph->AddEdge(4, 0);

    std::shared_ptr<IGraph> matrixGraph = std::make_shared<MatrixGraph>(*listGraph);
    std::shared_ptr<IGraph> arcGraph = std::make_shared<ArcGraph>(*matrixGraph);
    std::shared_ptr<IGraph> setGraph = std::make_shared<SetGraph>(*arcGraph);

    EXPECT_EQ(listGraph->VerticesCount(), matrixGraph->VerticesCount());
    EXPECT_EQ(matrixGraph->VerticesCount(), arcGraph->VerticesCount());
    EXPECT_EQ(arcGraph->VerticesCount(), setGraph->VerticesCount());

    EXPECT_EQ(listGraph->GetNextVertices(4).size(), matrixGraph->GetNextVertices(4).size());
    EXPECT_EQ(matrixGraph->GetNextVertices(4).size(), arcGraph->GetNextVertices(4).size());
    EXPECT_EQ(arcGraph->GetNextVertices(4).size(), setGraph->GetNextVertices(4).size());

    EXPECT_EQ(listGraph->GetPrevVertices(2).size(), matrixGraph->GetPrevVertices(2).size());
    EXPECT_EQ(matrixGraph->GetPrevVertices(2).size(), arcGraph->GetPrevVertices(2).size());
    EXPECT_EQ(arcGraph->GetPrevVertices(2).size(), setGraph->GetPrevVertices(2).size());
}
