#include <iostream>
#include <vector>

void dfs(std::vector<std::vector<int>> &graph, std::vector<bool> &visited, int v) {
    if (visited[v]) {
        return;
    }
    visited[v] = true;
    for (int vertice: graph[v]) {
        dfs(graph, visited, vertice);
    }
}

void run(std::istream &in, std::ostream &out) {
    int num_of_edges = 0;
    int num_of_vertices = 0;
    int len_of_path = 0;
    in >> num_of_vertices >> num_of_edges >> len_of_path;
    std::vector<std::vector<int>> graph(num_of_vertices);
    for (int i = 0; i < num_of_edges; ++i) {
        int a, b;
        in >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    std::vector<bool> visited(num_of_vertices, false);
    int counter = 0;
    for (int i = 0; i < num_of_vertices; ++i) {
        if (!visited[i]) {
            dfs(graph, visited, i);
            counter++;
        }
    }
    out << counter;
}

int main() {
    run(std::cin, std::cout);
}