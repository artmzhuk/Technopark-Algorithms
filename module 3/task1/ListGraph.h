
#include <vector>
#include "IGraph.h"

class ListGraph : public IGraph {
public:
    ListGraph(std::size_t vertices_count);

    ListGraph(const IGraph &graph);

    virtual ~ListGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) override;

    virtual int VerticesCount() const override;

    virtual std::vector<int> GetNextVertices(int vertex) const override;

    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> vertices;
};

