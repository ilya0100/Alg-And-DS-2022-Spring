/*  7_3. Binary MSD для long long.
    Дан массив неотрицательных целых 64-разрядных чисел. Количество чисел не больше 10^6. 
    Отсортировать массив методом MSD по битам (бинарный QuickSort). */

#include <iostream>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <string.h>



int partition(unsigned long long* arr, int size, int bit_num) {
    if (size <= 1) {
        return 0;
    }
    unsigned long long bit = 1;
    bit = bit << bit_num;

    int i = 0;
    int j = size - 1;

    while (i <= j) {
        for ( ; i < size && ((arr[i] & bit) == 0); ++i) {}
        for ( ; j >= 0 && ((arr[j] & bit) != 0); --j) {
        }

        if (i < j) {
            std::swap(arr[i++], arr[j--]);
        }
    }
    return i;
}


void binary_quick_sort(unsigned long long* arr, int size, int bit_num = 63) {
    int part = partition(arr, size, bit_num);
    if (bit_num == 0) {
        return;
    }
    if (part > 0) {
        binary_quick_sort(arr, part, bit_num - 1);
    }
    if (part + 1 < size) {
        binary_quick_sort(arr + part, size - part, bit_num - 1);
    }
}


void counting_sort(unsigned long long* arr, int size) {
    unsigned long long max = arr[0];
    for (int i = 1; i < size; ++i) {
        max = std::max(max, arr[i]);
    }

    int* c = new int[max];
    memset(c, max, sizeof(int));
    for (int i = 0; i < size; ++i) {
        ++c[arr[i]];
    }

    for (int i = 1; i < max; ++i) {
        c[i] += c[i - 1];
    }

    unsigned long long* b = new unsigned long long[size];
    for (int i = size - 1; i >= 0; --i) {
        b[--c[arr[i]]] = arr[i];
    }
    std::copy(b, b + size, arr);
    
    delete[] b;
    delete[] c;
}


void run(std::istream& input, std::ostream& output) {
    int n;
    input >> n;
    unsigned long long* arr = new unsigned long long[n];

    for (int i = 0; i < n; ++i) {
        input >> arr[i];
    }

    binary_quick_sort(arr, n);
    for (int i = 0; i < n; ++i) {
        output << arr[i] << " ";
    }

    delete[] arr;
}


void run_tests() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 4 1000000 7";
        run(input, output);
        std::cout << "Test 1:" << std::endl;
        std::cout << output.str() << std::endl;
        assert(output.str() == "4 7 1000000 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 10 1 9 16 14";
        run(input, output);
        std::cout << "Test 2:" << std::endl;
        std::cout << output.str() << std::endl;
        assert(output.str() == "1 9 10 14 16 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 10000 100 1000 900 10";
        run(input, output);
        std::cout << "Test 3:" << std::endl;
        std::cout << output.str() << std::endl;
        assert(output.str() == "10 100 900 1000 10000 ");
    }
}


int main() {
    // run_tests();
    run(std::cin, std::cout);
}
