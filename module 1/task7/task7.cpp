/* 7_3. Binary MSD для long long.
 * Дан массив неотрицательных целых 64-разрядных чисел. Количество чисел не больше 106.
 * Отсортировать массив методом MSD по битам (бинарный QuickSort).
 */

#include <iostream>
#include <cstdint>

size_t partition(uint64_t *arr, size_t l, size_t r, uint64_t pivot, int radix) {
    if (l == r || pivot == 0) {
        return 0;
    }
    size_t i = l;
    size_t j = r;
    while (i != j) {
        for (; (i < j) && arr[i] <= pivot; i++);
        for (; (i < j) && arr[j] > pivot; j--);
        std::swap(arr[i], arr[j]);
    }
    if (radix != 0) {
        partition(arr, l, i, (pivot & ~((uint64_t) 1 << radix)) | ((uint64_t) 1 << (radix - 1)), radix - 1); // 0xxxxxxx
        partition(arr, j, r, pivot | ((uint64_t) 1 << (radix - 1)), radix - 1); //1xxxxxxx
    }
    return i;
}

void binary_quicksort(uint64_t *arr, size_t arr_size) {
    partition(arr, 0, arr_size - 1, (uint64_t) 1 << 63, 63);
}

void run(std::istream &in, std::ostream &out) {
    size_t arr_size = 0;
    in >> arr_size;
    auto *arr = new uint64_t[arr_size];
    for (size_t i = 0; i < arr_size; ++i) {
        in >> arr[i];
    }
    binary_quicksort(arr, arr_size);
    for (size_t i = 0; i < arr_size; ++i) {
        out << arr[i] << " ";
    }
    delete[] arr;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}