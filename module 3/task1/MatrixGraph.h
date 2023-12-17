#ifndef TECHNOPARK_ALGORITHMS_MATRIXGRAPH_H
#define TECHNOPARK_ALGORITHMS_MATRIXGRAPH_H
#endif //TECHNOPARK_ALGORITHMS_MATRIXGRAPH_H

#include <vector>
#include <cstdint>
#include "IGraph.h"


class MatrixGraph : public IGraph {
public:
    MatrixGraph(std::size_t vertices_count);

    MatrixGraph(const IGraph &graph);

    virtual ~MatrixGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) override;

    virtual int VerticesCount() const override;

    virtual std::vector<int> GetNextVertices(int vertex) const override;

    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<uint8_t>> matrix;
};
