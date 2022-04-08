#include <iostream>
#include <cassert>


void read_array(int *arr, int size) {
    assert(arr != nullptr);

    for (int i = 0; i < size; ++i) {
        std::cin >> arr[i];
    }
}

void print_array(int *arr, int size) {
    assert(arr != nullptr);

    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
}

int exp_bin_search(int *arr, int size, int value) {
    assert(arr != nullptr);

    int begin = 0;
    int end = 1;
    while (end < size && arr[end] < value) {
        begin = end;
        end <<= 1;
    }
    if (end > size) {
        end = size;
    }

    while (begin < end) {
        int middle = (end + begin) / 2;
        if (arr[middle] < value) {
            begin = middle + 1;
        } else {
            end = middle;
        }
    }
    
    if (begin == size) {
        return size -1;
    } else if (begin == 0 || value == arr[begin]) {
        return begin;
    } else {
        return value - arr[begin - 1] <= arr[begin] - value ? begin - 1 : begin;
    }
}

void find_near_numbers(int *arr, int n, int *search_arr, int m, int *result) {
    assert(arr != nullptr && search_arr != nullptr && result != nullptr);
    
    for (int i = 0; i < m; ++i) {
        result[i] = exp_bin_search(arr, n, search_arr[i]);
    }
}

int main() {
    int n = 0;
    int m = 0;

    std::cin >> n;
    int *A = new int[n];
    assert(A != nullptr);
    read_array(A, n);

    std::cin >> m;
    int *B = new int[m];
    assert(B != nullptr);
    read_array(B, m);

    int *result = new int[m];
    assert(result != nullptr);

    find_near_numbers(A, n, B, m, result);
    print_array(result, m);

    delete[] A;
    delete[] B;
    delete[] result;
    return 0;
}