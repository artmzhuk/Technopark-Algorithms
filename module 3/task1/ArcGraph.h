
#include <vector>
#include "IGraph.h"


class ArcGraph : public IGraph {
public:
    ArcGraph(std::size_t vertices_count);

    ArcGraph(const IGraph &graph);

    virtual ~ArcGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) override;

    virtual int VerticesCount() const override;

    virtual std::vector<int> GetNextVertices(int vertex) const override;

    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector <std::pair<int, int>> edges;
    int num_of_vertices;
};
