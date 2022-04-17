/*  Напишите программу, которая использует кучу для слияния K отсортированных массивов суммарной длиной N.
    Требования: время работы O(N * logK). Ограничение на размер кучи O(K). */

#include <iostream>
#include <cassert>
#include <algorithm>
#include <sstream>

#define INIT_SIZE 40


template <class T>
class Array {
 public:
    Array() = default;
    Array(const size_t n);

    explicit Array(const Array& other);
    Array& operator=(const Array& other);

    ~Array();

    T operator[](const size_t i) const;
    T& operator[](const size_t i);

    size_t size() const { return data_size; }

    void clear();

    void push_back(const T& value);
    T pop_back();

 private:
    T* data = nullptr;
    size_t data_size = 0;
    size_t capacity = 0;

    void grow();
};

template <class T>
struct Element {
    T value;
    size_t value_index;
    size_t array_index;

    Element(): value(0), value_index(0), array_index(0) {}
    Element(T val, size_t val_i, size_t arr_i): value(val), value_index(val_i), array_index(arr_i) {}
};

template <class T>
class IsMoreByElement {
 public:
    bool operator()(const Element<T>& l, const Element<T>& r) const { return l.value > r.value; }
};

template <class T>
class IsMoreDefault {
 public:
    bool operator()(const T& l, const T& r) const { return l > r; }
};

template <class T, class Compare = IsMoreDefault<T>>
class Heap {
 public:
    Heap(Compare cmp = (IsMoreDefault<T>())): cmp(cmp) {}
    explicit Heap(const Array<T>& arr,
                  Compare cmp = (IsMoreDefault<T>())): buffer(arr), cmp(cmp) { build_heap(); }

    void insert(const T& element);
    T pop();

    bool is_empty() const { return buffer.size() == 0; }

 private:
    Array<T> buffer;
    Compare cmp;

    void build_heap();
    void shift_down(size_t i);
    void shift_up(size_t i);
};


template <class T, class Compare>
void merge_arrays(Array<Array<T>>& arrays, Heap<Element<T>, Compare>& merge_heap, std::ostream& output) {
    for (size_t i = 0; i < arrays.size(); ++i) {
        merge_heap.insert(Element<int>(arrays[i][0], 0, i));
    }

    while (!merge_heap.is_empty()) {
        Element<T> temp = merge_heap.pop();
        if (temp.value_index + 1 < arrays[temp.array_index].size()) {
            merge_heap.insert(Element<T>(arrays[temp.array_index][temp.value_index + 1],
                                         temp.value_index + 1, temp.array_index));
        }
        output << temp.value << " ";
    }
}



int run(std::istream& input, std::ostream& output) {
    size_t k;
    input >> k;
    Array<Array<int>> arrays(k);

    for (size_t i = 0; i < k; ++i) {
        size_t array_size;
        input >> array_size;
        for (size_t j = 0; j < array_size; ++j) {
            int temp;
            input >> temp;
            arrays[i].push_back(temp);
        }
    }
    Heap<Element<int>, IsMoreByElement<int>> merge_heap((IsMoreByElement<int>()));

    merge_arrays(arrays, merge_heap, output);
    return 0;
}


void run_tests() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 1 6 2 50 90 3 1 10 70";
        run(input, output);
        assert(output.str() == "1 6 10 50 70 90 ");
    }
    {
        // один массив
        std::stringstream input;
        std::stringstream output;
        input << "1 5 6 8 30 50 55";
        run(input, output);
        assert(output.str() == "6 8 30 50 55 ");
    }
    {
        // все элементы следующего массива больше элементов предыдущего
        std::stringstream input;
        std::stringstream output;
        input << "3 3 6 8 10 4 15 22 29 30 2 50 70";
        run(input, output);
        assert(output.str() == "6 8 10 15 22 29 30 50 70 ");
    }
    {
        // аналогично, но массивы считываются в другом порядке
        std::stringstream input;
        std::stringstream output;
        input << "3 4 15 22 29 30 3 6 8 10 2 50 70";
        run(input, output);
        assert(output.str() == "6 8 10 15 22 29 30 50 70 ");
    }
    {
        // элементы первого массива должны находиться между элементами второго
        std::stringstream input;
        std::stringstream output;
        input << "2 5 2 4 6 8 10 5 1 3 5 7 9";
        run(input, output);
        assert(output.str() == "1 2 3 4 5 6 7 8 9 10 ");
    }
    {
        // 6 массивов с одним элементом
        std::stringstream input;
        std::stringstream output;
        input << "6 1 3 1 2 1 7 1 9 1 5 1 13";
        run(input, output);
        assert(output.str() == "2 3 5 7 9 13 ");
    }
    {
        // повторяющиеся элементы
        std::stringstream input;
        std::stringstream output;
        input << "3 3 1 5 10 2 5 7 5 7 8 9 10 15";
        run(input, output);
        assert(output.str() == "1 5 5 7 7 8 9 10 10 15 ");
    }
}


int main() {
    run(std::cin, std::cout);
    // run_tests();
    return 0;
}


template <class T>
Array<T>::Array(const size_t n): data_size(n), capacity(n) {
    data = new T[n];
    assert(data != nullptr);
}

template <class T>
Array<T>& Array<T>::operator=(const Array& other) {
    if (this != &other) {
        delete[] data;
        data = new T[other.data_size];

        std::copy(other.data, other.data + other.data_size, data);
        data_size = other.data_size;
        capacity = other.capacity;
    }
    return *this;
}

template <class T>
Array<T>::Array(const Array& other) {
    *this = other;
}

template <class T>
Array<T>::~Array() {
    delete[] data;
}

template <class T>
T Array<T>::operator[](const size_t i) const {
    assert(i < data_size);
    return data[i];
}

template <class T>
T& Array<T>::operator[](const size_t i) {
    assert(i < data_size);
    return data[i];
}

template <class T>
void Array<T>::clear() {
    if (data_size != 0) {
        delete[] data;
        data = nullptr;
        data_size = 0;
        capacity = 0;
    }
}

template <class T>
void Array<T>::push_back(const T& value) {
    if (data_size >= capacity) {
        grow();
    }

    data[data_size] = value;
    ++data_size;
}

template <class T>
T Array<T>::pop_back() {
    assert(data_size != 0);
    return data[--data_size];
}

template <class T>
void Array<T>::grow() {
    size_t new_capacity = std::max((size_t)INIT_SIZE, capacity * 2);
    T* new_data = new T[new_capacity];
    assert(new_data != nullptr);

    std::copy(data, data + data_size, new_data);
    delete[] data;

    data = new_data;
    capacity = new_capacity;
}


template <class T, class Compare>
void Heap<T, Compare>::insert(const T& element) {
    buffer.push_back(element);
    shift_up(buffer.size() - 1);
}

template <class T, class Compare>
T Heap<T, Compare>::pop() {
    assert(buffer.size() != 0);

    T result = buffer[0];
    buffer[0] = buffer.pop_back();

    if (buffer.size() != 0) {
        shift_down(0);
    } else {
        buffer.clear();
    }
    return result;
}

template <class T, class Compare>
void Heap<T, Compare>::build_heap() {
    for (size_t i = buffer.size() / 2 + 1; ; --i) {
        shift_down(i);
        if (i == 0) {
            break;
        }
    }
}

template <class T, class Compare>
void Heap<T, Compare>::shift_down(size_t i) {
    size_t left = 0;
    size_t right = 0;

    while (left < buffer.size() || right < buffer.size()) {
        left = 2 * i + 1;
        right = 2 * i + 2;
        size_t largest = i;

        if (left < buffer.size() && cmp(buffer[largest], buffer[left])) {
            largest = left;
        }
        if (right < buffer.size() && cmp(buffer[largest], buffer[right])) {
            largest = right;
        }
        if (largest == i) { break; }
        std::swap(buffer[i], buffer[largest]);
        i = largest;
    }
}

template <class T, class Compare>
void Heap<T, Compare>::shift_up(size_t i) {
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (cmp(buffer[i], buffer[parent])) {
            return;
        }
        std::swap(buffer[i], buffer[parent]);
        i = parent;
    }
}
