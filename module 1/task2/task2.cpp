/*
 * 2_1. Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
 * Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A, равного
 * или превосходящего B[i]: A[k] >= B[i]. Если такого элемента нет, выведите n. n, m ≤ 10000.
 * Требования:  Время работы поиска k для каждого элемента B[i]: O(log(k)).
 * ! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k с
 * помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.
*/

#include <iostream>

int BinSearch(int *A, int target, int left, int right){
    while(left < right){
        int middle = (left + right) / 2;
        if(target > A[middle]) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }
    if(A[left] >= target){
        return left;
    } else {
        return left + 1;
    }
}

int ExpSearch(int *A, int target, int n){
    if(A[n-1] < target){
        return n;
    }
    int right = 1;
    while(right < n && A[right] <= target){
        right *= 2;
    }
    return BinSearch(A, target, right / 2, std::min(right, n - 1));
}


int main(){
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    int *A = new int[n];
    int *B = new int[m];
    for(int i = 0; i < n; ++i){
        std::cin >> A[i];
    }
    for(int i = 0; i < m; ++i){
        std::cin >> B[i];
    }

    for(int i = 0; i < m; ++i){
        std::cout << ExpSearch(A, B[i], n) << " ";
    }

    delete[] A;
    delete[] B;
    return 0;
}
