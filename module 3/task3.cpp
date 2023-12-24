/*Требуется отыскать самый выгодный маршрут между городами.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <climits>

struct Edge {
    int to;
    int weight;

    Edge(int to, int weight) : to(to), weight(weight) {}
};

struct PairComp {
    constexpr bool operator()(
            std::pair<int, int> const &a,
            std::pair<int, int> const &b)
    const noexcept {
        return a.second > b.second;
    }
};


class Graph {
public:
    Graph(std::size_t vertices_count) {
        vertices.resize(vertices_count);
    }

    void AddEdge(int from, int to, int weight) {
        Edge edge(to, weight);
        vertices[from].push_back(edge);
    }

    size_t VerticesCount() const {
        return vertices.size();
    }

    std::vector<int> Dijkstra(int u) {
        std::vector<int> dist(VerticesCount(), INT_MAX);
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, PairComp> queue;
        dist[u] = 0;
        queue.emplace(u, 0);
        while (!queue.empty()) {
            int popped = queue.top().first;
            int distance_to_popped = queue.top().second;
            queue.pop();

            if (distance_to_popped >= dist[popped]) {
                for (Edge &edge: vertices[popped]) {
                    if (dist[edge.to] > dist[popped] + edge.weight) {
                        dist[edge.to] = dist[popped] + edge.weight;
                        queue.emplace(edge.to, dist[edge.to]);
                    }
                }
            }
        }
        return dist;
    }

private:
    std::vector<std::vector<Edge>> vertices;
};


void run(std::istream &in, std::ostream &out) {
    int num_of_vertices = 0;
    int num_of_edges = 0;
    in >> num_of_vertices >> num_of_edges;
    Graph graph(num_of_vertices);
    for (int i = 0; i < num_of_edges; ++i) {
        int first_edge = 0;
        int second_edge = 0;
        int weight = 0;
        in >> first_edge >> second_edge >> weight;
        graph.AddEdge(first_edge, second_edge, weight);
        if (first_edge != second_edge) {
            graph.AddEdge(second_edge, first_edge, weight);
        }
    }
    int first_city = 0;
    int second_city = 0;
    in >> first_city >> second_city;
    std::vector<int> result = graph.Dijkstra(first_city);
    out << result[second_city] << std::endl;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}