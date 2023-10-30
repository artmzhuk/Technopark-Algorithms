/* 6_2. Реализуйте стратегию выбора опорного элемента “медиана трёх”.
 * Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.
 */

#include <iostream>


template<class T>
struct DefaultComparator {
    bool operator()(const T &l, const T &r) const {
        return l < r;
    }
};

template<class T, typename Comp = DefaultComparator<T>>
long long median_idx(T *arr, long long idx_a, long long idx_b, long long idx_c, Comp comp = Comp()) { //bubblesort for 3 elems
    if (!(comp(arr[idx_b], arr[idx_a])) && !(comp(arr[idx_c], arr[idx_b]))) return idx_b;
    if (!(comp(arr[idx_c], arr[idx_a])) && !(comp(arr[idx_b], arr[idx_c]))) return idx_c;
    if (!(comp(arr[idx_a], arr[idx_b])) && !(comp(arr[idx_c], arr[idx_a]))) return idx_a;
    if (!(comp(arr[idx_c], arr[idx_b])) && !(comp(arr[idx_a], arr[idx_c]))) return idx_c;
    if (!(comp(arr[idx_a], arr[idx_c])) && !(comp(arr[idx_b], arr[idx_a]))) return idx_a;
    return idx_b;
}

template<typename T, typename Comp>
long long partition(T *arr, long long l, long long r, Comp comp) {
    if(r == 1){
        return 0;
    }

    long long pivot_idx = median_idx(arr, 0, l + (r - l) / 2, r - 1);
    long long pivot = arr[pivot_idx];

    std::swap(arr[r - 1], arr[pivot_idx]);
    pivot_idx = r - 1;


    long long i = r - 2;
    long long j = r - 2;

    while (j >= 0) {
        if(comp(pivot, arr[j])){
            std::swap(arr[i], arr[j]);
            --i;
            --j;
        } else {
            --j;
        }
    }
    std::swap(arr[pivot_idx], arr[i + 1]);
    return i + 1;
}

template<class T, class Comp=DefaultComparator<T>>
void kth_element(T *arr, long long size, long long k, Comp comp = Comp()) {
    long long l = 0;
    long long r = size;
    while (l < r) {
        int m = partition(arr, l, r, comp);
        if (k == m) {
            return;
        } else if (k < m) {
            r = m;
        } else {
            l = m + 1;
        }
    }
}

void run(std::istream &in, std::ostream &out) {
    long long arr_size = 0;
    in >> arr_size;
    if (arr_size == 0) {
        out << "0\n0\n0\n";
        return;
    }
    auto *arr = new long long[arr_size];
    for (long long i = 0; i < arr_size; ++i) {
        in >> arr[i];
    }
    long long _10th_percent = arr_size / 10;
    long long medium = arr_size / 2;
    long long _90th_percent = (arr_size * 9) / 10;
    kth_element(arr, arr_size, _10th_percent);
    kth_element(arr, arr_size, medium);
    kth_element(arr, arr_size, _90th_percent);
    out << arr[_10th_percent] << "\n" << arr[medium] << "\n" << arr[_90th_percent];
    delete[] arr;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}