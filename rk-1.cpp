#include <iostream>
#include <cassert>


int find_element(int *arr, int size, int value) {
    assert(arr != nullptr);

    int begin = 0;
    int end = size;

    while (begin < end) {
        int middle = (begin + end) / 2;
        if (arr[middle] < value) {
            begin = middle + 1;
        } else {
            end = middle;
        }
    }

    if (begin == size) {
        return size;
    } else {
        return begin;
    }
}

int main() {
    int n;
    std::cin >> n;

    int *A = new int[n];
    assert(A != nullptr);

    for (int i = 0; i < n; ++i) {
        std::cin >> A[i];
    }
    int k;
    std::cin >> k;

    std::cout << find_element(A, n, k);

    delete[] A;
    return 0;
}
