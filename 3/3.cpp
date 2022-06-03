#include <set>
#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>



class WeightedListGraph {
 public:
    WeightedListGraph(size_t verticesCount);

    WeightedListGraph(const WeightedListGraph& other);
    WeightedListGraph& operator=(const WeightedListGraph& other);

    void AddEdge(int from, int to, int weight);

    int VerticesCount() const;

    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const;
    std::vector<std::pair<int, int>> GetPrevVertices(int vertex) const;

 private:
    std::vector<std::vector<std::pair<int, int>>> adjList;
};


int dijkstra(const WeightedListGraph& graph, int from, int to) {
    assert(from >= 0 && from < graph.VerticesCount());
    assert(to >= 0 && to < graph.VerticesCount());

    std::vector<int> parents(graph.VerticesCount(), -1);
    std::vector<int> dist(graph.VerticesCount(), INT32_MAX);
    std::set<std::pair<int, int>> queue;

    dist[from] = 0;
    queue.insert({0, from});

    while (queue.size() != 0) {
        std::pair<int, int> current = queue.extract(queue.begin()).value();

        for (auto next : graph.GetNextVertices(current.second)) {
            if (dist[next.second] == INT32_MAX) {
                dist[next.second] = dist[current.second] + next.first;
                parents[next.second] = current.second;

                queue.insert({dist[next.second], next.second});

            } else if (dist[next.second] > dist[current.second] + next.first) {
                queue.erase({dist[next.second], next.second});
                queue.insert({dist[current.second] + next.first, next.second});

                dist[next.second] = dist[current.second] + next.first;
                parents[next.second] = current.second;
            }
        }
    }
    return dist[to];
}



void run(std::istream& input, std::ostream& output) {
    int verticesCount;
    input >> verticesCount;
    WeightedListGraph graph(verticesCount);

    int edgesCount;
    input >> edgesCount;
    for (int i = 0; i < edgesCount; ++i) {
        int from;
        int to;
        int weight;
        input >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
        graph.AddEdge(to, from, weight);
    }
    int from;
    int to;
    input >> from >> to;

    output << dijkstra(graph, from, to);
}

void run_tests() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "6 9 0 3 1 0 4 2 1 2 7 1 3 2 1 4 3 1 5 3 2 5 3 3 4 4 3 5 6 0 2";
        run(input, output);

        std::cout << "***** Test 1: *****" << std::endl;
        std::cout << output.str() << std::endl;

        assert(output.str() == "9");

        std::cout << "***** Sucess *****\n" << std::endl;
    }
}


int main() {
    // run_tests();
    run(std::cin, std::cout);
}



WeightedListGraph::WeightedListGraph(size_t verticesCount) {
    adjList.resize(verticesCount);
}

WeightedListGraph::WeightedListGraph(const WeightedListGraph& other) {
    adjList.resize(other.VerticesCount());
    for (int i = 0; i < other.VerticesCount(); ++i) {
        adjList[i] = other.GetNextVertices(i);
    }
}

WeightedListGraph& WeightedListGraph::operator=(const WeightedListGraph& other) {
    *this = WeightedListGraph(other);
    return *this;
}

void WeightedListGraph::AddEdge(int from, int to, int weight) {
    assert(from >= 0 && from < adjList.size());
    assert(to >= 0 && to < adjList.size());

    adjList[from].push_back({weight, to});
}

int WeightedListGraph::VerticesCount() const {
    return adjList.size();
}

std::vector<std::pair<int, int>> WeightedListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjList.size());
    return adjList[vertex];
}

std::vector<std::pair<int, int>> WeightedListGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjList.size());

    std::vector<std::pair<int, int>> prevVertices;
    for (size_t from = 0; from < adjList.size(); ++from) {
        for (auto to : adjList[from]) {
            if (to.second == vertex) {
                prevVertices.push_back({to.first, from});
                break;
            }
        }
    }
    return prevVertices;
}
