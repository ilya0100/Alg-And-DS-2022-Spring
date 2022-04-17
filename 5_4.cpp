/*  5_4. Закраска прямой 2.
    На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов каждого
    отрезка (Li и Ri). Найти сумму длин частей числовой прямой, окрашенных ровно в один слой. */

#include <iostream>
#include <cassert>
#include <algorithm>
#include <sstream>


template <class T>
struct Pair {
    T first;
    T second;
};


template <class T>
class IsLessDefault {
 public:
    bool operator()(const T& l, const T& r) const { return l < r; }
};


template <typename T>
class IsLessPair {
 public:
    bool operator()(const Pair<T>& l, const Pair<T> r) const { return l.first < r.first; }
};


template <class T, class Compare = IsLessDefault<T>>
void merge_two_arrays(T* l, const size_t l_size, T* r, const size_t r_size,
                      T* dest, Compare cmp = IsLessDefault<T>()) {
    size_t dest_it = 0;
    size_t l_it = 0;
    size_t r_it = 0;

    while (l_it < l_size && r_it < r_size) {
        if (cmp(r[r_it], l[l_it])) {
            dest[dest_it++] = r[r_it];
            ++r_it;
        } else {
            dest[dest_it++] = l[l_it];
            ++l_it;
        }
    }
    while (l_it < l_size) {
        dest[dest_it++] = l[l_it++];
    }
    while (r_it < r_size) {
        dest[dest_it++] = r[r_it++];
    }
}


template <class T, class Compare = IsLessDefault<T>>
void merge_sort(T* arr, size_t size, Compare cmp = IsLessDefault<T>()) {
    T* buffer = new T[size];
    for (size_t i = 1; i < size; i *= 2) {
        for (size_t j = 0; j + i < size; j += 2 * i) {
            size_t second_size = i;

            // если остался последний кусочек размер которого меньше i, то определяем его размер
            if (j + 2 * i > size) {
                second_size = i - (j + 2 * i - size);
            }
            merge_two_arrays(arr + j, i, arr + j + i, second_size, buffer, cmp);
            std::copy(buffer, buffer + i + second_size, arr + j);
        }
    }
    delete[] buffer;
}


void run(std::istream& input, std::ostream& output) {
    size_t n;
    input >> n;

    Pair<int>* pair_arr = new Pair<int>[2 * n];
    for (size_t i = 0; i < 2 * n; ++i) {
        input >> pair_arr[i].first;
        pair_arr[i].second = 1 - 2 * (i % 2);
    }

    merge_sort(pair_arr, 2 * n, IsLessPair<int>());

    size_t length = 0;
    size_t layers = 0;
    for (size_t i = 0; i < 2 * n; ++i) {
        if (layers == 1 && layers + pair_arr[i].second != 1) {
            length += pair_arr[i].first - pair_arr[i - 1].first;
        }
        layers += pair_arr[i].second;
    }
    output << length;

    delete[] pair_arr;
}


void run_tests() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 1 4 7 8 2 5";
        run(input, output);
        assert(output.str() == "3");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4 6 7 0 3 2 5 8 11";
        run(input, output);
        assert(output.str() == "8");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 8 11 12 14 10 16";
        run(input, output);
        assert(output.str() == "5");
    }
}


int main() {
    // run_tests();
    run(std::cin, std::cout);
}
