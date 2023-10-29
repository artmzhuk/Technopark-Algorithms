#include <iostream>
#include <cmath>

template<typename T>
struct DefaultComparator {
    bool operator()(const T &l, const T &r) {
        return l < r;
    }
};


template<typename T, typename Comparator>
void Merge(T *a, size_t med, size_t right, T *tmp_array) {
    Comparator compare;
    size_t left_counter = 0;
    size_t right_counter = med;
    size_t tmp_counter = 0;
    while (tmp_counter < right) {
        if (right_counter < right &&
            (left_counter == med || compare(a[right_counter], a[left_counter]))) {

            tmp_array[tmp_counter] = a[right_counter];
            right_counter++;
        } else {
            tmp_array[tmp_counter] = a[left_counter];
            left_counter++;
        }
        tmp_counter++;
    }
}

template<typename T, typename Comparator = DefaultComparator<T>>
void Mergesort(T *a, size_t a_length) {
    if (a_length <= 1) {
        return;
    }
    size_t first_length = a_length / 2;
    size_t second_length = a_length - first_length;
    Mergesort<T, Comparator>(a, first_length);
    Mergesort<T, Comparator>(a + first_length, second_length);
    T *tmp_array = new T[a_length];
    Merge<T, Comparator>(a, first_length, a_length, tmp_array);
    for (size_t i = 0; i < a_length; ++i) {
        a[i] = tmp_array[i];
    }
    delete[] tmp_array;
}

struct Node {
    int x; // x coordinate
    int y;
    double angle;
};

struct NodeComparator {
    bool operator()(const Node &l, const Node &r) {
        if(l.x != r.x){
            return l.x < r.x;
        } else {
            return l.y < r.y;
        }
    }
};

struct NodeComparator2 {
    bool operator()(const Node &l, const Node &r) {
        return l.angle < r.angle;
    }
};

void run(std::istream &in, std::ostream &out) {
    size_t num_of_nodes = 0;
    in >> num_of_nodes;

    Node *nodes = new Node[num_of_nodes];
    for (size_t i = 0; i < num_of_nodes; i++) {
        in >> nodes[i].x >> nodes[i].y;
    }
    Mergesort<Node, NodeComparator>(nodes, num_of_nodes);
    for(size_t i = 0; i < num_of_nodes; i++){
        nodes[i].angle = atan(double((abs(nodes[0].x - nodes[i].x))) / double(abs((nodes[0].y - nodes[i].y))));
    }
    Mergesort<Node, NodeComparator2>(nodes, num_of_nodes);
    for(size_t i = 0; i < num_of_nodes; i++){
        out << nodes[i].x << " " << nodes[i].y << "\n";
    }
    delete[] nodes;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}