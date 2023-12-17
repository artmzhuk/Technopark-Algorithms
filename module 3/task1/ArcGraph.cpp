#include "ArcGraph.h"


ArcGraph::ArcGraph(std::size_t vertices_count) {
    num_of_vertices = vertices_count;
}

ArcGraph::ArcGraph(const IGraph &graph) {
    num_of_vertices = graph.VerticesCount();
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        for (auto to: graph.GetNextVertices(from)) {
            edges.emplace_back(from, to);
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    edges.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
    return num_of_vertices;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    for (std::pair<int, int> edge: edges) {
        if (edge.first == vertex) {
            result.push_back(edge.second);
        }
    }
    return result;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;
    for (std::pair<int, int> edge: edges) {
        if (edge.second == vertex) {
            result.push_back(edge.first);
        }
    }
    return result;
}
