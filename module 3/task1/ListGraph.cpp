

#include "ListGraph.h"

ListGraph::ListGraph(std::size_t vertices_count) {
    vertices.resize(vertices_count);
}

ListGraph::ListGraph(const IGraph &graph) {
    vertices.resize(graph.VerticesCount());
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        for (auto to: graph.GetNextVertices(from)) {
            vertices[from].push_back(to);
        }
    }
}

void ListGraph::AddEdge(int from, int to) {
    vertices[from].push_back(to);
}

int ListGraph::VerticesCount() const {
    return vertices.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    for (int v: vertices[vertex]) {
        result.push_back(v);
    }
    return result;
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;

    for (int from = 0; from < vertices.size(); ++from) {
        for (int to: vertices[from]) {
            if (to == vertex) {
                result.push_back(from);
            }
        }
    }
    return result;
}
