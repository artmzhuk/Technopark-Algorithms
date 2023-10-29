#include <iostream>

int BinSearch(int *A, int left, int right) {
    while (left < right) {
        int middle = (left + right) / 2;
        if (A[left] <= A[middle]) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }
    if (A[left] >= A[left + 1]) {
        return left + 1;
    } else {
        return left;
    }
}

void run(std::istream &in, std::ostream &out) {
    int arr_size = 0;
    in >> arr_size;
    int *arr = new int[arr_size];

    for (int i = 0; i < arr_size; i++) {
        std::cin >> arr[i];
    }
    out << BinSearch(arr, 0, arr_size);
    delete[] arr;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}