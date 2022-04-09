#include <iostream>
#include <cassert>
#include <algorithm>

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

    void resize(const size_t new_size);
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
class IsLessDefault {
 public:
    bool operator()(const T& l, const T& r) const { return l < r; }
};

template <class T, class Compare = IsLessDefault<T>>
class Heap {
 public:
    Heap(Compare cmp = IsLessDefault<T>()): cmp(cmp) {};
    explicit Heap(const Array<T>& arr,
                  Compare cmp = IsLessDefault<T>()): buffer(arr), cmp(cmp) { build_heap(); }

    void insert(const T& element);
    T pop();
    // T peek() const;

 private:
    Array<T> buffer;
    Compare cmp;

    void build_heap();
    void shift_down(size_t i);
    void shift_up(size_t i);
};


int main() {
    std::cout << "Enter size: " << std::endl;
    size_t n;
    std::cin >> n;

    Array<int> arr(n);
    std::cout << "Enter elements: " << std::endl;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    Heap<int> h(arr);
    h.insert(3);
 
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
void Array<T>::resize(const size_t new_size) {
    T* new_data = new T[new_size];
    assert(new_data != nullptr);

    std::copy(data, data + std::min(data_size, new_size), new_data);
    delete[] data;

    data = new_data;
    data_size = new_size;
    capacity = new_size;
}

template <class T>
void Array<T>::clear() {
    if (data_size != 0) {
        delete[] data;
        data = nullptr;
        data_size = 0;
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
    return data[data_size--];
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
    assert(buffer.data_size != 0);

    T result = buffer[0];
    buffer[0] = buffer.pop_back();

    if (buffer.data_size != 0) {
        shift_down(0);
    }
    return result;
}

template <class T, class Compare>
void Heap<T, Compare>::build_heap() {
    size_t i = buffer.size() / 2;
    while (i > 0) {
        --i;
        shift_down(i);
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
        if (right < buffer.size() && buffer[right] > buffer[largest]) {
            largest = left;
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
        if (buffer[i] < buffer[parent]) {
            return;
        }
        std::swap(buffer[i], buffer[parent]);
        i = parent;
    }
}
