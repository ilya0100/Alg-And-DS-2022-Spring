/*  Реализовать очередь с динамическим зацикленным буфером.
    Обрабатывать команды push back и pop front. */

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

 private:
    T* data = nullptr;
    size_t data_size = 0;
};


template <class T>
class Queue {
 public:
    Queue() = default;

    void push_back(T value);
    T pop_front();

    bool is_empty() const { return buffer.size() == 0; }

 private:
    Array<T> buffer;

    size_t head = 0;
    size_t tail = 0;

    void resize_buffer();
};



int main() {
    int operations_count;
    std::cin >> operations_count;
    Queue<int> queue;
    bool is_values_correct = true;

    for (int i = 0; i < operations_count; ++i) {
        int choice;
        std::cin >> choice;
        int value;

        switch (choice) {
        case 2:
            int test_value;
            std::cin >> test_value;

            if (queue.is_empty()) {
                value = -1;
            } else {
                value = queue.pop_front();
            }

            if (value != test_value) {
                is_values_correct = false;
            }
            break;

        case 3:
            std::cin >> value;
            queue.push_back(value);
            break;

        default:
            break;
        }
    }
    std::cout << (is_values_correct ? "YES" : "NO");
    return 0;
}



template <class T>
Array<T>::Array(const size_t n): data_size(n) {
    data = new T[n];
    assert(data != nullptr);
}

template <class T>
Array<T>& Array<T>::operator=(const Array& other) {
    if (this != &other) {
        delete[] data;
        data = new T[other.size()];

        std::copy(other.data, other.data + other.size(), data);
        data_size = other.data_size;
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
void Queue<T>::push_back(T value) {
    if (is_empty() || tail == head) {
        resize_buffer();
    }

    buffer[tail] = value;
    tail = (tail + 1) % buffer.size();
}

template <class T>
T Queue<T>::pop_front() {
    assert(!is_empty());

    T temp = buffer[head];
    head = (head + 1) % buffer.size();

    if (head == tail) {
        buffer.clear();
    }
    return temp;
}

template <class T>
void Queue<T>::resize_buffer() {
    Array<T> temp(buffer);

    size_t new_buffer_size = std::max((size_t)INIT_SIZE, buffer.size() * 2);
    buffer.resize(new_buffer_size);

    for (size_t i = 0; i < temp.size(); ++i) {
        buffer[i] = temp[head];
        head = (head + 1) % temp.size();

        if (head == tail) {
            break;
        }
    }

    head = 0;
    tail = temp.size();
}
