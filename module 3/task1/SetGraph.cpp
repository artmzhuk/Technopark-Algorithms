

#include "SetGraph.h"

SetGraph::SetGraph(std::size_t vertices_count) {
    vertices.resize(vertices_count);
}

SetGraph::SetGraph(const IGraph &graph) {
    vertices.resize(graph.VerticesCount());
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        for (auto to: graph.GetNextVertices(from)) {
            vertices[from].insert(to);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    vertices[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return vertices.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    for (int v: vertices[vertex]) {
        result.push_back(v);
    }
    return result;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
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
