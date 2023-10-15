/*5_3. Закраска прямой 1.
 * На числовой прямой окрасили N отрезков. Известны координаты левого и правого
 * концов каждого отрезка (Li и Ri).
 * Найти длину окрашенной части числовой прямой.
*/

#include <iostream>

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
    unsigned long long x; // x coordinate
    bool is_start;
};

struct NodeComparator {
    bool operator()(const Node &l, const Node &r) {
        return l.x < r.x;
    }
};

void run(std::istream &in, std::ostream &out) {
    size_t num_of_segments;
    in >> num_of_segments;
    size_t num_of_nodes = num_of_segments * 2;

    Node *nodes = new Node[num_of_nodes];
    for (size_t i = 0; i < num_of_nodes; i += 2) {
        in >> nodes[i].x >> nodes[i + 1].x;
        nodes[i].is_start = true;
        nodes[i + 1].is_start = false;
    }
    Mergesort<Node, NodeComparator>(nodes, num_of_nodes);
    int num_of_layers = 1;
    size_t color_part_length = 0;
    size_t start_of_color_segment = nodes[0].x;
    for (int i = 1; i < num_of_nodes; ++i) {
        if (num_of_layers == 0) {
            start_of_color_segment = nodes[i].x;
        }
        if (nodes[i].is_start) {
            num_of_layers++;
        } else {
            num_of_layers--;
        }
        if (num_of_layers == 0) {
            color_part_length += nodes[i].x - start_of_color_segment;
        }
    }
    out << color_part_length;
    delete[] nodes;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}
