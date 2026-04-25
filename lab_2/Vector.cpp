#include "Vector.h"
#include <iostream>

Vector::Vector(const ValueType *rawArray, const size_t size, float coef) {
    _size = size;
    _multiplicativeCoef = coef;
    _capacity = static_cast<size_t>(_multiplicativeCoef * static_cast<float>(size));

    _data = new ValueType[_capacity];
    for (int i = 0; i < _size; ++i) {
        _data[i] = rawArray[i];
    }
}

Vector::Vector(const Vector &other) {
    _size = other._size;
    _multiplicativeCoef = other._multiplicativeCoef;
    _capacity = static_cast<size_t>(_multiplicativeCoef * static_cast<float>(_size));

    _data = new ValueType[_capacity];
    for (size_t i = 0; i < _size; ++i) {
        _data[i] = other._data[i];
    }
}

Vector &Vector::operator=(const Vector &other) {
    if (this == &other) {
        return *this;
    }

    _capacity = other._capacity;
    _multiplicativeCoef = other._multiplicativeCoef;
    _size = other._size;

    delete[] _data;
    _data = new ValueType[_capacity];
    for (int i = 0; i < _size; ++i) {
        _data[i] = other._data[i];
    }
    return *this;
}

Vector::Vector(Vector &&other) noexcept {
    _size = other._size;
    _multiplicativeCoef = other._multiplicativeCoef;
    _capacity = other._capacity;
    _data = other._data;

    other._size = 0;
    other._capacity = 0;
    other._data = nullptr;
}

Vector &Vector::operator=(Vector &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    _size = other._size;
    _multiplicativeCoef = other._multiplicativeCoef;
    _capacity = other._capacity;

    delete[] _data;
    _data = other._data;

    other._size = 0;
    other._capacity = 0;
    other._data = nullptr;

    return *this;
}

Vector::~Vector() {
    delete []_data;
}

void Vector::pushBack(const ValueType &value) {
    if (_size == _capacity) {
        size_t newCapacity = _capacity == 0 ?
            static_cast<size_t>(_multiplicativeCoef) :
            static_cast<size_t>(_capacity * _multiplicativeCoef);
        reserve(newCapacity);
    }
    _data[_size++] = value;
}

void Vector::reserve(const size_t capacity) {
    if (capacity > _capacity) {
        const ValueType *prev_data = _data;
        _capacity = capacity;

        _data = new ValueType[_capacity];
        for (int i = 0; i < _size; ++i) {
            _data[i] = prev_data[i];
        }

        delete[] prev_data;
    }
}

void Vector::pushFront(const ValueType &value) {
    if (_size == _capacity) {
        size_t newCapacity = _capacity == 0 ?
            static_cast<size_t>(_multiplicativeCoef) :
            static_cast<size_t>(_capacity * _multiplicativeCoef);
        reserve(newCapacity);
    }
    for (size_t i = _size; i > 0; --i) {
        _data[i] = _data[i - 1];
    }
    _data[0] = value;
    _size++;

}

void Vector::insert(const ValueType &value, size_t pos) {
    for (size_t i = _size; i > pos; --i) {
        _data[i] = _data[i - 1];
    }
    _data[pos] = value;
    _size++;
    if (_size == _capacity) {
        reserve(_capacity * 2);
    }
}

void Vector::insert(const ValueType *values, size_t size, size_t pos) {
    if (_size + size >= _capacity) {
        reserve((_size + size) * 2);
    }
    for (size_t i = _size + size - 1; i >= size + pos; --i) {
        _data[i] = _data[i - size];
    }
    for (size_t i = pos; i < pos + size; i++) {
        _data[i] = values[i - pos];
    }
    _size = _size + size;
}

void Vector::insert(const Vector &vector, size_t pos) {
    insert(vector._data, vector._size, pos);
}

void Vector::popBack() {
    if (_size == 0) {
        throw std::out_of_range("Vector is empty");
    }
    _size--;
}

void Vector::popFront() {
    if (_size == 0) {
        throw std::out_of_range("Vector is empty");
    }
    for (size_t i = 0; i < _size - 1; i++) {
        _data[i] = _data[i + 1];
    }
    _size--;
}

void Vector::erase(size_t pos, size_t count) {
    if (pos >= _size) {
        throw std::out_of_range("Position out of range");
    }
    if (pos + count > _size) {
        count = _size - pos;
    }

    for (size_t i = pos; i < _size - count; i++) {
        _data[i] = _data[i + count];
    }
    _size -= count;
}

void Vector::eraseBetween(size_t beginPos, size_t endPos) {
    size_t count = endPos - beginPos;
    erase(beginPos, count);
}

size_t Vector::size() const {
    return _size;
}

size_t Vector::capacity() const {
    return _capacity;
}

double Vector::loadFactor() const {
    return static_cast<double>(_size) / static_cast<double>(_capacity);
}

ValueType &Vector::operator[](size_t idx) {
    return _data[idx];
}

const ValueType &Vector::operator[](size_t idx) const {
    return _data[idx];
}

long long Vector::find(const ValueType &value) const {
    for (int i = 0; i < _size; i++) {
        if (_data[i] == value) {
            return i;
        }
    }
    return -1;
}

void Vector::shrinkToFit() {
    const ValueType *prev_data = _data;
    _capacity = _size;

    _data = new ValueType[_size];
    for (int i = 0; i < _size; ++i) {
        _data[i] = prev_data[i];
    }

    delete[] prev_data;
}

Vector::Iterator::Iterator(ValueType *ptr) {
    _ptr = ptr;
}

ValueType &Vector::Iterator::operator*() {
    return *_ptr;
}

const ValueType &Vector::Iterator::operator*() const {
    return *_ptr;
}

ValueType *Vector::Iterator::operator->() {
    return _ptr;
}

const ValueType *Vector::Iterator::operator->() const {
    return _ptr;
}

Vector::Iterator Vector::Iterator::operator++() {
    ++_ptr;
    return *this;
}

Vector::Iterator Vector::Iterator::operator++(int) {
    Iterator temp = *this;
    ++_ptr;
    return temp;
}

bool Vector::Iterator::operator==(const Iterator &other) const {
    return _ptr == other._ptr;
}

bool Vector::Iterator::operator!=(const Iterator &other) const {
    return _ptr != other._ptr;
}

Vector::Iterator Vector::begin() {
    return Iterator(_data);
}

Vector::Iterator Vector::end() {
    return Iterator(&_data[_size]);
}
