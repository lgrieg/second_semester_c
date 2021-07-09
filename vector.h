#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <cstddef>
#include <algorithm>
#include <iostream>

class VectorOutOfRange : public std::out_of_range {
public:
    VectorOutOfRange() : std::out_of_range("VectorOutOfRange"){};
};

template <class T>
class Vector {
public:
    T* buffer;
    size_t vector_size;
    size_t capacity;

    Vector() {
        buffer = nullptr;
        vector_size = 0;
        capacity = 0;
    }

    explicit Vector(size_t size) {
        buffer = new T[size];
        capacity = size;
        vector_size = size;
    }

    Vector(size_t size, const T& value) {
        buffer = new T[size];
        for (size_t i = 0; i < size; ++i) {
            buffer[i] = value;
        }
        vector_size = size;
        capacity = size;
    }

    Vector(const Vector& other) : vector_size(other.vector_size), capacity(other.capacity) {
        if (capacity == 0) {
            buffer = nullptr;
        } else {
            buffer = new T[capacity];
            for (size_t i = 0; i < vector_size; ++i) {
                buffer[i] = other.buffer[i];
            }
        }
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            T* new_buffer = new T[other.capacity];
            for (size_t i = 0; i < other.vector_size; ++i) {
                new_buffer[i] = other.buffer[i];
            }

            if (buffer != nullptr) {
                delete[] buffer;
            }
            buffer = new_buffer;
            vector_size = other.vector_size;
            capacity = other.capacity;
        }
        return *this;
    }

    void Clear() {
        vector_size = 0;
    }

    void Resize(size_t new_size) {
        if (new_size > capacity) {
            T* new_buffer = new T[new_size];
            for (size_t i = 0; i < vector_size; ++i) {
                new_buffer[i] = buffer[i];
            }

            if (buffer != nullptr) {
                delete[] buffer;
            }
            buffer = new_buffer;
            capacity = new_size;
        }
        vector_size = new_size;
    }

    void Resize(size_t new_size, T value) {
        if (new_size > capacity) {
            std::cout << "realloc" << std::endl;
            T* new_buffer = new T[new_size];
            for (size_t i = 0; i < vector_size; ++i) {
                new_buffer[i] = buffer[i];
            }

            if (buffer != nullptr) {
                delete[] buffer;
            }
            buffer = new_buffer;
            capacity = new_size;
        }
        if (new_size > vector_size) {
            for (size_t j = vector_size; j < new_size; ++j) {
                buffer[j] = value;
            }
        }
        vector_size = new_size;
    }

    void PushBack(const T& value) {
        if (capacity == 0) {
            size_t old_size = vector_size;
            Resize(1);
            vector_size = old_size;
        } else if (vector_size == capacity) {
            size_t old_size = vector_size;
            Resize(capacity * 2);
            vector_size = old_size;
        }
        buffer[vector_size] = value;
        ++vector_size;
    }

    void PopBack() {
        --vector_size;
    }

    void Reserve(const size_t& new_cap) {
        if (new_cap > capacity) {
            T* new_buffer = new T[new_cap];
            for (size_t i = 0; i < vector_size; ++i) {
                new_buffer[i] = buffer[i];
            }
            if (buffer != nullptr) {
                delete[] buffer;
            }
            buffer = new_buffer;
            capacity = new_cap;
        }
    }

    void ShrinkToFit() {
        if (capacity > vector_size) {
            T* new_buffer = new T[vector_size];
            for (size_t i = 0; i < vector_size; ++i) {
                new_buffer[i] = buffer[i];
            }
            if (buffer != nullptr) {
                delete[] buffer;
            }
            buffer = new_buffer;
            capacity = vector_size;
        }
    }

    void Swap(Vector& other) {
        std::swap(vector_size, other.vector_size);
        std::swap(capacity, other.capacity);
        std::swap(buffer, other.buffer);
    }

    T operator[](const size_t& index) const {
        return buffer[index];
    }

    T& operator[](const size_t& index) {
        return buffer[index];
    }

    const T At(const size_t& index) const {
        if (index < 0 || index >= vector_size) {
            throw VectorOutOfRange{};
        }
        return buffer[index];
    }

    T& At(const size_t& index) {
        if (index < 0 || index >= vector_size) {
            throw VectorOutOfRange{};
        }
        return buffer[index];
    }

    T Front() const {
        return buffer[0];
    }

    T& Front() {
        return buffer[0];
    }

    T Back() const {
        return buffer[vector_size - 1];
    }

    T& Back() {
        return buffer[vector_size - 1];
    }

    bool Empty() const {
        return vector_size == 0;
    }

    size_t Size() const {
        return vector_size;
    }

    size_t Capacity() const {
        return capacity;
    }

    T* Data() {
        return buffer;
    }

    ~Vector() {
        if (buffer != nullptr) {
            delete[] buffer;
        }
    }
};

template <class T>
bool operator<(const Vector<T>& a, const Vector<T>& b) {
    for (size_t i = 0; i < std::min(a.Size(), b.Size()); ++i) {
        if (a.buffer[i] == b.buffer[i]) {
            continue;
        }
        return a.buffer[i] < b.buffer[i];
    }
    return a.Size() < b.Size();
}

template <class T>
bool operator>(const Vector<T>& a, const Vector<T>& b) {
    return b < a;
}

template <class T>
bool operator>=(const Vector<T>& a, const Vector<T>& b) {
    return !(a < b);
}

template <class T>
bool operator<=(const Vector<T>& a, const Vector<T>& b) {
    return !(a > b);
}

template <class T>
bool operator==(const Vector<T>& a, const Vector<T>& b) {
    return (b <= a) && (b >= a);
}

template <class T>
bool operator!=(const Vector<T>& a, const Vector<T>& b) {
    return !(b == a);
}

#endif  // VECTOR_VECTOR_H
