#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <iostream>
#include <stdexcept>

template<class T>
class Vector {
private:
    std::vector<T> data;

public:
    Vector() {}

    Vector(size_t initialSize) : data(initialSize) {}

    Vector(size_t initialSize, const T& defaultValue) : data(initialSize, defaultValue) {}

    size_t getSize() const {
        return data.size();
    }

    void push_back(const T& value) {
        data.push_back(value);
    }

    T& operator[](size_t index) {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    void clear() {
        data.clear();
    }
};

#endif

