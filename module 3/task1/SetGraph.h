#include <vector>
#include <cstdint>
#include <unordered_set>
#include "IGraph.h"


class SetGraph : public IGraph {
public:
    SetGraph(std::size_t vertices_count);

    SetGraph(const IGraph &graph);

    virtual ~SetGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) override;

    virtual int VerticesCount() const override;

    virtual std::vector<int> GetNextVertices(int vertex) const override;

    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::unordered_set<int>> vertices;
};
