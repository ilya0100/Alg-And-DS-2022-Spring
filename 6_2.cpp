/*  6_2. Реализуйте стратегию выбора опорного элемента “медиана трёх”.
    Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу. */

#include <iostream>
#include <cassert>
#include <algorithm>
#include <sstream>


template <class T>
class IsLessDefault {
 public:
    bool operator()(const T& l, const T& r) const { return l < r; }
};


template <class T, class Compare = IsLessDefault<T>>
size_t median_of_three(T* arr, size_t i, size_t j, size_t k, Compare cmp = IsLessDefault<T>()) {
    T first = arr[i];
    T second = arr[j];
    T third = arr[k];

    // если первый элемент больше воторого или третьего и если первый меньше второго или третьего
    // то он является медианой
    if (cmp(second, first) || cmp(third, first)) {
        if (!cmp(second, first) || !cmp(third, first)) {
            return i;
        }
    }
    if (cmp(first, second) || cmp(third, second)) {
        if (!cmp(first, second) || !cmp(third, second)) {
            return j;
        }
    }
    if (cmp(second, third) || cmp(first, third)) {
        if (!cmp(second, third) || !cmp(first, third)) {
            return k;
        }
    }
    return i;
}


template <class T, class Compare = IsLessDefault<T>>
size_t partition(T* arr, size_t l, size_t r, Compare cmp = IsLessDefault<T>()) {
    if (l >= r) {
        return l;
    }

    std::swap(arr[median_of_three(arr, l, (l + r) / 2, r - 1, cmp)], arr[l]);
    T pivot = arr[l];
    size_t i = r - 1;
    size_t j = i;

    for (; j > l; --j) {
        if (cmp(pivot, arr[j])) {
            std::swap(arr[j], arr[i--]);
        }
    }
    std::swap(arr[l], arr[i]);
    return i;
}


template <class T, class Compare = IsLessDefault<T>>
T find_k_stat(T* arr, size_t n, size_t k, Compare cmp = IsLessDefault<T>()) {
    size_t l = 0;
    size_t r = n;
    size_t pivot_pos;

    do {
        pivot_pos = partition(arr, l, r, cmp);
        if (k > pivot_pos) {
            l = pivot_pos + 1;
        } else {
            r = pivot_pos;
        }
    } while (pivot_pos != k);

    return arr[k];
}


void run(std::istream& input, std::ostream& output) {
    size_t n;
    input >> n;
    int* arr = new int[n];

    for (size_t i = 0; i < n; ++i) {
        input >> arr[i];
    }

    size_t percentile_10 = n * 10 / 100;
    size_t percentile_90 = n * 90 / 100;
    size_t median = n * 50 / 100;

    output << find_k_stat(arr, n, percentile_10) << std::endl;
    output << find_k_stat(arr, n, median) << std::endl;
    output << find_k_stat(arr, n, percentile_90);
    delete[] arr;
}


void run_tests() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 1 2 3 4 5 6 7 8 9 10";
        run(input, output);
        assert(output.str() == "2\n6\n10");
    }
}


int main() {
    // run_tests();
    run(std::cin, std::cout);
}
