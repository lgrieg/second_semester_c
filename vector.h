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

    size_t vector_size_;
    size_t capacity_;
    T* buffer_;

public:
    Vector() : vector_size_(0), capacity_(0), buffer_(nullptr){};

    explicit Vector(size_t size) : vector_size_(size), capacity_(size), buffer_(new T[size]){};

    void FillBuffer(size_t start_id, size_t end_id, const T& value) {
        for (size_t i = start_id; i < end_id; ++i) {
            buffer_[i] = value;
        }
    }

    Vector(size_t size, const T& value) : Vector(size) {
        // buffer_ = new T[size];
        FillBuffer(0, size, value);
        // vector_size_ = size;
        // capacity_ = size;
    }

    void Copy(const Vector& other) {
        other.CopyTo(buffer_);
    }

    void CopyTo(T* new_buffer) const {
        for (size_t i = 0; i < vector_size_; ++i) {
            new_buffer[i] = buffer_[i];
        }
    }

    void ReplaceBuffer(T* new_buffer) {
        delete[] buffer_;
        buffer_ = new_buffer;
    }

    void CreateBufferCopyReplaceUpdCapacity(size_t new_capacity) {
        T* new_buffer = new T[new_capacity];
        CopyTo(new_buffer);
        ReplaceBuffer(new_buffer);
        capacity_ = new_capacity;
    }

    Vector(const Vector& other)
        : vector_size_(other.vector_size_)
        , capacity_(other.capacity_)
        , buffer_(capacity_ == 0 ? nullptr : new T[capacity_]) {
        Copy(other);
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            if (capacity_ < other.capacity_) {
                T* new_buffer = new T[other.capacity_];
                ReplaceBuffer(new_buffer);
            }
            Copy(other);
            vector_size_ = other.vector_size_;
            capacity_ = other.capacity_;
        }
        return *this;
    }

    void Clear() {
        vector_size_ = 0;
    }

    void Resize(size_t new_size) {
        if (new_size > capacity_) {
            CreateBufferCopyReplaceUpdCapacity(new_size);
        }
        vector_size_ = new_size;
    }

    void Resize(size_t new_size, T value) {
        const size_t old_vector_size = vector_size_;
        Resize(new_size);
        if (new_size > old_vector_size) {
            FillBuffer(old_vector_size, new_size, value);
        }
    }

    void PushBack(const T& value) {
        if (capacity_ == 0) {
            Reserve(1);
            vector_size_ = 0;
        } else if (vector_size_ == capacity_) {
            const size_t old_size = vector_size_;
            Reserve(capacity_ * 2);
            vector_size_ = old_size;
        }
        buffer_[vector_size_] = value;
        ++vector_size_;
    }

    void PopBack() {
        --vector_size_;
    }

    void Reserve(size_t new_cap) {
        if (new_cap > capacity_) {
            CreateBufferCopyReplaceUpdCapacity(new_cap);
        }
    }

    void ShrinkToFit() {
        if (capacity_ > vector_size_) {
            CreateBufferCopyReplaceUpdCapacity(vector_size_);
        }
    }

    void Swap(Vector& other) {
        std::swap(vector_size_, other.vector_size_);
        std::swap(capacity_, other.capacity_);
        std::swap(buffer_, other.buffer_);
    }

    T operator[](size_t index) const {
        return buffer_[index];
    }

    T& operator[](size_t index) {
        return buffer_[index];
    }

    const T At(size_t index) const {
        if (index >= vector_size_) {
            throw VectorOutOfRange{};
        }
        return buffer_[index];
    }

    T& At(size_t index) {
        if (index >= vector_size_) {
            throw VectorOutOfRange{};
        }
        return buffer_[index];
    }

    T Front() const {
        return buffer_[0];
    }

    T& Front() {
        return buffer_[0];
    }

    T Back() const {
        return buffer_[vector_size_ - 1];
    }

    T& Back() {
        return buffer_[vector_size_ - 1];
    }

    bool Empty() const {
        return vector_size_ == 0;
    }

    size_t Size() const {
        return vector_size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    T* Data() {
        return buffer_;
    }

    const T* Data() const {
        return buffer_;
    }

    ~Vector() {
        delete[] buffer_;
    }
};

template <class T>
bool operator<(const Vector<T>& a, const Vector<T>& b) {
    const size_t min_size = std::min(a.Size(), b.Size());
    for (size_t i = 0; i < min_size; ++i) {
        if (a[i] < b[i]) {
            return true;
        }
        if (a[i] > b[i]) {
            return false;
        }
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
    const size_t a_size = a.Size();
    const size_t b_size = b.Size();
    if (a_size == b_size) {
        for (size_t i = 0; i < a_size; ++i) {
            if (a[i] != b[i]) {
                return false;
            }
        }
    }
    return a_size == b_size;
}

template <class T>
bool operator!=(const Vector<T>& a, const Vector<T>& b) {
    return !(b == a);
}

#endif  // VECTOR_VECTOR_H
