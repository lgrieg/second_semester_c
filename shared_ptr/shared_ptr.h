#ifndef SHARED_PTR_SHARED_PTR_H
#define SHARED_PTR_SHARED_PTR_H
#define MAKE_SHARED_IMPLEMENTED

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <iostream>

class BadWeakPtr : public std::runtime_error {
public:
    BadWeakPtr() : std::runtime_error("BadWeakPtr") {
    }
};

template <class T>
class WeakPtr;

struct Counter {
    size_t shared;
    size_t weak;

    Counter() : shared(0), weak(0){};
    Counter(size_t shared_param, size_t weak_param) : shared(shared_param), weak(weak_param){};
};

template <class T>
class SharedPtr {
    T* ptr_;
    Counter* counter_;

    void DecreaseCounter() {
        if (!counter_) {
            return;
        }
        if (counter_->shared > 0) {
            if (counter_->shared == 1) {
                delete ptr_;
            }
            --counter_->shared;
        }
        if (counter_->shared == 0 && counter_->weak == 0) {
            delete counter_;
        }
    }

public:
    SharedPtr() : ptr_(nullptr), counter_(nullptr){};
    SharedPtr(T* other) : ptr_(other), counter_(ptr_ ? new Counter(1, 0) : nullptr){};   // NOLINT
    SharedPtr(const SharedPtr<T>& other) : ptr_(other.ptr_), counter_(other.counter_) {  // NOLINT
        if (counter_) {
            ++counter_->shared;
        }
    }
    SharedPtr(SharedPtr<T>&& other) : ptr_(other.ptr_), counter_(other.counter_) {  // NOLINT
        other.ptr_ = nullptr;
        other.counter_ = nullptr;
    }

    SharedPtr<T>& operator=(T* other) {
        if (ptr_ != other) {
            Reset(other);
        }
        return *this;
    }

    SharedPtr<T>& operator=(const SharedPtr<T>& other) {
        if (this != &other) {
            SharedPtr<T> copy_ptr(other);
            Swap(copy_ptr);
        }
        return *this;
    }

    SharedPtr<T>& operator=(SharedPtr<T>&& other) {
        if (this != &other) {
            SharedPtr<T> move_ptr(std::move(other));
            Swap(move_ptr);
        }
        return *this;
    }

    explicit SharedPtr<T>(const WeakPtr<T>& other) : ptr_(other.ptr_), counter_(other.counter_) {  // NOLINT
        if (other.Expired()) {
            throw BadWeakPtr{};
        }
        ++counter_->shared;
    }

    void Reset(T* other = nullptr) {
        DecreaseCounter();
        ptr_ = other;
        counter_ = (ptr_ ? new Counter(1, 0) : nullptr);
    }

    void Swap(SharedPtr<T>& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(counter_, other.counter_);
    }

    T* Get() const {
        return ptr_;
    }

    size_t UseCount() const {
        return counter_ ? counter_->shared : 0;
    }

    T& operator*() const {
        return *ptr_;
    }

    T* operator->() const {
        return ptr_;
    }

    explicit operator bool() const {
        return ptr_;
    }

    ~SharedPtr() {
        DecreaseCounter();
    }

    friend class WeakPtr<T>;
};

template <class T>
class WeakPtr {
    T* ptr_;
    Counter* counter_;

    void DecreaseCounter() {
        if (!counter_) {
            return;
        }
        if (counter_->weak > 0) {
            --counter_->weak;
        }
        if (counter_->shared == 0 && counter_->weak == 0) {
            delete counter_;
        }
    }

public:
    WeakPtr() : ptr_(nullptr), counter_(nullptr){};
    WeakPtr(const WeakPtr<T>& other) : ptr_(other.ptr_), counter_(other.counter_) {  // NOLINT
        if (counter_) {
            ++counter_->weak;
        }
    }
    WeakPtr(WeakPtr<T>&& other) : ptr_(other.ptr_), counter_(other.counter_) {  // NOLINT
        other.ptr_ = nullptr;
        other.counter_ = nullptr;
    }

    WeakPtr(const SharedPtr<T>& other) : ptr_(other.ptr_), counter_(other.counter_) {  // NOLINT
        if (counter_) {
            ++counter_->weak;
        }
    }

    WeakPtr<T>& operator=(const WeakPtr<T>& other) {
        if (this != &other) {
            WeakPtr<T> copy_ptr(other);
            Swap(copy_ptr);
        }
        return *this;
    }

    WeakPtr<T>& operator=(WeakPtr<T>&& other) {
        if (this != &other) {
            WeakPtr<T> move_ptr(std::move(other));
            Swap(move_ptr);
        }
        return *this;
    }

    void Swap(WeakPtr<T>& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(counter_, other.counter_);
    }

    void Reset() {
        DecreaseCounter();
        ptr_ = nullptr;
        counter_ = nullptr;
    }

    size_t UseCount() {
        return counter_ ? counter_->shared : 0;
    }

    bool Expired() const {
        return counter_ == nullptr || counter_->shared == 0;
    }

    SharedPtr<T> Lock() {
        return Expired() ? SharedPtr<T>(nullptr) : SharedPtr<T>(*this);
    }

    ~WeakPtr() {
        DecreaseCounter();
    }

    friend class SharedPtr<T>;
};

template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... params) {
    return SharedPtr<T>(new T(std::forward<Args>(params)...));
}

#endif  //  UNIQUE_PTR_UNIQUE_PTR_H
