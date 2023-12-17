#include <iostream>
#include <random>
#include <cassert>
#include <algorithm>
#include "ArcGraph.h"
#include "SetGraph.h"
#include "ListGraph.h"
#include "MatrixGraph.h"

int main(){
    std::random_device seed;
    std::mt19937 gen{seed()}; // seed the generator
    std::uniform_int_distribution<> dist{0, 999}; // set min and max
    ListGraph list(1000);
    for(int i = 0; i < list.VerticesCount(); ++i){
        int guess = dist(gen);
        list.AddEdge(i, guess);
    }
    MatrixGraph matrix(list);
    SetGraph set(matrix);
    ArcGraph arc(set);
    ListGraph list2(arc);
    assert(list.VerticesCount() == matrix.VerticesCount());
    assert(matrix.VerticesCount() == set.VerticesCount());
    assert(set.VerticesCount() == arc.VerticesCount());
    assert(arc.VerticesCount() == list2.VerticesCount());
    assert(list2.VerticesCount() == list.VerticesCount());
    for(int i = 0; i < list.VerticesCount(); ++i){
        std::vector<int> list_next = list.GetNextVertices(i);
        std::vector<int> matrix_next = matrix.GetNextVertices(i);
        std::vector<int> set_next = set.GetNextVertices(i);
        std::vector<int> arc_next = arc.GetNextVertices(i);
        std::vector<int> list2_next = list2.GetNextVertices(i);
        sort(list_next.begin(), list_next.end());
        sort(matrix_next.begin(), matrix_next.end());
        sort(set_next.begin(), set_next.end());
        sort(arc_next.begin(), arc_next.end());
        sort(list2_next.begin(), list2_next.end());
        assert(list.GetNextVertices(i) == matrix.GetNextVertices(i));
        assert(matrix.GetNextVertices(i) == set.GetNextVertices(i));
        assert(set.GetNextVertices(i) == arc.GetNextVertices(i));
        assert(arc.GetNextVertices(i) == list2.GetNextVertices(i));
        assert(list2.GetNextVertices(i) == list.GetNextVertices(i));

        std::vector<int> list_prev = list.GetPrevVertices(i);
        std::vector<int> matrix_prev = matrix.GetPrevVertices(i);
        std::vector<int> set_prev = set.GetPrevVertices(i);
        std::vector<int> arc_prev = arc.GetPrevVertices(i);
        std::vector<int> list2_prev = list2.GetPrevVertices(i);
        sort(list_prev.begin(), list_prev.end());
        sort(matrix_prev.begin(), matrix_prev.end());
        sort(set_prev.begin(), set_prev.end());
        sort(arc_prev.begin(), arc_prev.end());
        sort(list2_prev.begin(), list2_prev.end());
        assert(list.GetPrevVertices(i) == matrix.GetPrevVertices(i));
        assert(matrix.GetPrevVertices(i) == set.GetPrevVertices(i));
        assert(set.GetPrevVertices(i) == arc.GetPrevVertices(i));
        assert(arc.GetPrevVertices(i) == list2.GetPrevVertices(i));
        assert(list2.GetPrevVertices(i) == list.GetPrevVertices(i));
    }
    std::cout << "assert OK\n";
    return 0;
}