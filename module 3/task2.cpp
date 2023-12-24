/*Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами.
 * Найдите количество различных кратчайших путей между заданными вершинами.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <climits>

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

    void AddEdge(int from, int to) {
        vertices[from].push_back(to);
    }

    size_t VerticesCount() const {
        return vertices.size();
    }

    std::vector<int> GetNextVertices(int vertex) const {
        std::vector<int> result;
        for (int v: vertices[vertex]) {
            result.push_back(v);
        }
        return result;
    }

    std::vector<int> GetPrevVertices(int vertex) const {
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

    int CountNumOfDifferentWays(int u, int w) {
        std::vector<int> dist(VerticesCount(), INT_MAX);
        std::vector<int> path_counter(VerticesCount(), 0);
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, PairComp> queue;
        path_counter[u] = 1;
        dist[u] = 0;
        queue.emplace(u, 0);
        while (!queue.empty()) {
            int popped = queue.top().first;
            int distance_to_popped = queue.top().second;
            queue.pop();

            if(distance_to_popped >= dist[popped]){
                for (int &neighbour: vertices[popped]) {
                    if (dist[neighbour] > dist[popped] + 1) {
                        dist[neighbour] = dist[popped] + 1;
                        path_counter[neighbour] = path_counter[popped];
                        queue.emplace(neighbour, dist[neighbour]);
                    } else if (dist[neighbour] == dist[popped] + 1) {
                        path_counter[neighbour] += path_counter[popped];
                    }
                }
            }
        }
        return path_counter[w];
    }

private:
    std::vector<std::vector<int>> vertices;
};


void run(std::istream &in, std::ostream &out) {
    int num_of_vertices = 0;
    int num_of_edges = 0;
    in >> num_of_vertices >> num_of_edges;
    Graph graph(num_of_vertices);
    for (int i = 0; i < num_of_edges; ++i) {
        int first_edge = 0;
        int second_edge = 0;
        in >> first_edge >> second_edge;
        graph.AddEdge(first_edge, second_edge);
        graph.AddEdge(second_edge, first_edge);
    }
    int u = 0;
    int w = 0;
    in >> u >> w;
    out << graph.CountNumOfDifferentWays(u, w);
}

int main() {
    run(std::cin, std::cout);
    return 0;
}