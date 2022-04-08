#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>

#define INIT_SIZE 40

template <class T>
class Array {
public:
    Array() = default;
    Array(const size_t n);
    Array(const Array&) = delete;
    ~Array();

    Array& operator=(const Array&) = delete;

    T operator[](const size_t i) const;
    T& operator[](const size_t i);

    size_t size() const { return length; }
    T* get_pointer() const { return data; }
    
    void grow();
    void add_elem(const T value);

private:
    T* data = nullptr;
    size_t length = 0;
    size_t capacity = 0;
};


template <class T>
class Queue {
public:
    Queue() = default;

    void push_back(T value);
    T pop_front();

    bool is_empty() const { return head == tail; }

private:
    Array<T> data;

    size_t head;    
    size_t tail;    
};


int main() {
    Array<int> a(10);
    a.grow();
    std::cout << a.size();

    std::vector<int> b(5);
    b.size();
    return 0;
}


template <class T>
Array<T>::Array(const size_t n): length(n), capacity(n) {
    data = new T[n];
    assert(data != nullptr);
}

template <class T>
Array<T>::~Array() {
    assert(data != nullptr);
    delete[] data;
}

template <class T>
T Array<T>::operator[](const size_t i) const {
    assert(i < length);
    return data[i];
}

template <class T>
T& Array<T>::operator[](const size_t i) {
    assert(i < length);
    return data[i];
}

template <class T>
void Array<T>::grow() {
    size_t new_capacity = std::max(capacity * 2, (size_t) INIT_SIZE);

    T* new_data = new T[new_capacity];
    assert(new_data != nullptr);
    
    std::copy(data, data + length, new_data);
    delete[] data;
    data = new_data;
    capacity = new_capacity;
}
