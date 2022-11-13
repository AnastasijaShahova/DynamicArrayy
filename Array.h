#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>

#define DEFAULT_CAPACITY  8;
#define DOUBLE_CAPACITY 2;

template<typename T>
class Array final {
public:
    Array() {
        size_ = 0;
        capacity_ = DEFAULT_CAPACITY;
        data_ = (T*)std::malloc(capacity_ * sizeof(T));
    }

    Array(int capacity) {
        size_ = 0;
        capacity_ = capacity;
        data_ = (T*)std::malloc(capacity_ * sizeof(T));
    }

    Array(const Array<T>& array) {
        capacity_ = array.capacity_;
        size_ = array.size_;
        data_ = (T*)std::malloc(sizeof(T) * capacity_);

        for (int i = 0; i < size_; i++) {
            new(data_ + i) T(array.data_[i]);
        }
    }

    Array(Array<T>&& array) {
        capacity_ = array.capacity_;
        size_ = array.size_;
        data_ = array.data_;
        array.data_ = nullptr;
        array.size_ = 0;
    }

    Array& operator=(const Array<T>& array) {
        Array<T> temp(array);
        std::swap(data_, temp.data_);
        std::swap(size_, temp.size_);
        std::swap(capacity_, temp.capacity_);

        return *this;
    }

    Array& operator=(Array<T>&& array) {
        for (int i = 0; i < size_; ++i)
            data_[i].~T();

        std::free(data_);

        capacity_ = array.capacity_;
        size_ = array.size_;
        data_ = array.data_;

        array.size_ = 0;
        array.capacity_ = 0;
        array.data_ = nullptr;

        return *this;
    }

    ~Array() {
        for (int i = 0; i < size_; ++i)
            data_[i].~T();

        std::free(data_);
    }

    int insert(const T& value) {
        if (size_ >= capacity_) {

            capacity_ *= DOUBLE_CAPACITY;
            T *temp = (T*)std::malloc(capacity_ * sizeof(T));

            for (int i = 0; i < size_; ++i) {
                new(temp + i) T(std::move(data_[i]));
                data_[i].~T();
            }

            std::free(data_);
            data_ = temp;
        }

        new(data_ + size_) T(value);
        size_ += 1;
        return size_ - 1;
    }

    int insert(int index, const T& value) {
        if (size_ >= capacity_) {

            capacity_ = capacity_ * DOUBLE_CAPACITY;
            T *temp = (T*)std::malloc(capacity_ * sizeof(T));

            for (int i = 0; i < size_; ++i) {
                new(temp + i) T(std::move(data_[i]));
                data_[i].~T();
            }

            std::free(data_);
            data_ = temp;
        }

        if (index < size_) {

            for (int i = size_; i > index; --i){
                new(data_ + i) T(std::move(data_[i - 1]));
            }

            data_[index].~T();
        }

        new(data_ + index) T(value);
        size_ += 1;
        return index;
    }

    void remove(int index) {
        if (index >= size_ || index < 0)
            return;

        for (int i = index; i < size_ - 1; ++i) {
            data_[i].~T();
            new (data_ + i) T(std::move(data_[i + 1]));
        }
        data_[size_ - 1].~T();
        size_ -= 1;
    }

    const T& operator[](int index) const {
        return data_[index];
    }

    T& operator[](int index) {
        return data_[index];
    }

    int size() const {
        return size_;
    }

    int capacity() const {
        return capacity_;
    }

    class Iterator final {
    public:
        friend Array;
        T& get() const {
            return (*array_)[index_];
        }

        void set(const T& value) {
            (*array_)[index_] = value;
        }

        void next() {
            index_ += direction_;
        }

        bool hasNext() const {
            return (index_ + direction_ >= 0) && (index_ + direction_ < array_->size_);
        }

    private:
        Array* array_;
        int direction_;
        int index_;
    };

    class ConstIterator final {
    public:
        friend Array;
        const T& get() const {
            return (*array_)[index_];
        }

        void next() {
            index_ += direction_;
        }

        bool hasNext() const {
            return (index_ + direction_ >= 0) && (index_ + direction_ < array_->size_);
        }

    private:
        const Array* array_;
        int direction_;
        int index_;
    };

    Iterator iterator() {
        Iterator it;
        it.direction_ = 1;
        it.index_ = 0;
        it.array_ = this;

        return it;
    }

    Iterator reverseIterator() {
        Iterator it;
        it.direction_ = -1;
        it.index_ = size_ - 1;
        it.array_ = this;

        return it;
    }

    ConstIterator iterator() const {
        ConstIterator const_it;
        const_it.direction_ = 1;
        const_it.index_ = 0;
        const_it.array_ = this;

        return const_it;
    }

    ConstIterator reverseIterator() const {
        Iterator it;
        it.direction_ = -1;
        it.index_ = size_ - 1;
        it.data_ = this;

        return it;
    }

private:
    int capacity_;
    int size_;
    T* data_;
};

#endif // ARRAY_H
