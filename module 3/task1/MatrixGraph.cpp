#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(std::size_t vertices_count) {
    matrix.resize(vertices_count);
    for (auto i: matrix) {
        i.resize(vertices_count, 0);
    }
}

MatrixGraph::MatrixGraph(const IGraph &graph) {
    matrix.resize(graph.VerticesCount());
    for (auto &i: matrix) {
        i.resize(graph.VerticesCount(), 0);
    }
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        for (auto to: graph.GetNextVertices(from)) {
            matrix[from][to] = 1;
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    matrix[from][to] = 1;
}

int MatrixGraph::VerticesCount() const {
    return matrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    for (int i = 0; i < matrix[vertex].size(); ++i) {
        if (matrix[vertex][i] == 1) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;
    for (int i = 0; i < matrix.size(); ++i) {
        if (matrix[i][vertex] == 1) {
            result.push_back(i);
        }
    }
    return result;
}
