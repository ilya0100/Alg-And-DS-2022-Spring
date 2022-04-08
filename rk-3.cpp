#include <iostream>
#include <cassert>

void sort(int *arr, int size) {
    assert(arr != nullptr);

    for (int i = 1; i < size; ++i) {
        for (int j = i; j > 0 && arr[j - 1] > arr[j]; --j) {
            int temp = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = temp;
        }
    }
}

void sort_with_idexes(int *arr, int *indexes, int size) {
    assert(arr != nullptr && indexes != nullptr);

    for (int i = 1; i < size; ++i) {
        for (int j = i; j > 0 && arr[j - 1] > arr[j]; --j) {
            int temp = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = temp;

            temp = indexes[j];
            indexes[j] = indexes[j - 1];
            indexes[j - 1] = temp;
        }
    }
}


int main() {
    int n;
    std::cin >> n;
    int *arr = new int[n];
    int *indexes = new int[n];

    for (int i = 0; i < n; ++i) {
        indexes[i] = i;
    }

    for (int i = 0; i < n; ++i) {
        int temp[3];
        for (int j = 0; j < 3; ++j) {
            std::cin >> temp[j];
        }

        sort(temp, 3);
        arr[i] = temp[0];
    }

    sort_with_idexes(arr, indexes, n);

    for (int i = 0; i < n; ++i) {
        std::cout << indexes[i] << " ";
    }

    delete[] arr;
    delete[] indexes;
    return 0;
}
