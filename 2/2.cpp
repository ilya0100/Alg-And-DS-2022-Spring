#include <queue>
#include <iostream>
#include <sstream>
#include <cassert>



struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};


class ListGraph : public IGraph {
 public:
    ListGraph(size_t verticesCount);

    ListGraph(const IGraph& other);
    ListGraph& operator=(const IGraph& other);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const  override;
    std::vector<int> GetPrevVertices(int vertex) const override;

 private:
    std::vector<std::vector<int>> adjList;
};



int minPathCount(const IGraph& graph, int from, int to) {
    assert(from >= 0 && from < graph.VerticesCount());
    assert(to >= 0 && to < graph.VerticesCount());

    std::vector<int> dist(graph.VerticesCount(), INT32_MAX);
    std::vector<int> pathsCount(graph.VerticesCount(), 0);
    std::queue<int> bfsQueue;

    bfsQueue.push(from);
    pathsCount[from] = 1;
    dist[from] = 0;
    while (bfsQueue.size() > 0) {
        int current = bfsQueue.front();
        bfsQueue.pop();
        if (current == to) {
            return pathsCount[to];
        }

        for (auto next : graph.GetNextVertices(current)) {
            if (dist[next] > dist[current] + 1) {
                dist[next] = dist[current] + 1;
                pathsCount[next] = pathsCount[current];
                bfsQueue.push(next);
            } else if (dist[next] == dist[current] + 1) {
                pathsCount[next] += pathsCount[current];
            }
        }
    }
    return pathsCount[to];
}



void run(std::istream& input, std::ostream& output) {
    int verticesCount;
    input >> verticesCount;
    IGraph* graph = new ListGraph(verticesCount);

    int edgesCount;
    input >> edgesCount;
    for (int i = 0; i < edgesCount; ++i) {
        int from;
        int to;
        input >> from >> to;
        graph->AddEdge(from, to);
        graph->AddEdge(to, from);
    }
    int from;
    int to;
    input >> from >> to;
    output << minPathCount(*graph, from, to);

    delete graph;
}

void run_tests() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "4 5 0 1 0 2 1 2 1 3 2 3 0 3";
        run(input, output);

        std::cout << "***** Test 1: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "2");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
}


int main() {
    // run_tests();
    run(std::cin, std::cout);
}



ListGraph::ListGraph(size_t verticesCount) {
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
    for (size_t from = 0; from < adjList.size(); ++from) {
        for (size_t i  = 0; i < adjList[from].size(); ++i) {
            if (adjList[from][i] == vertex) {
                prevVertices.push_back(from);
                break;
            }
        }
    }
    return prevVertices;
}
