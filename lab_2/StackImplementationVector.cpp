#pragma once
#include "StackImplementation.h"
#include "Vector.h"


class StackImplementationVector : public IStackImplementation {
    Vector _data;

public:
    StackImplementationVector() {
        _data = Vector();
    }

    StackImplementationVector(const StackImplementationVector &vector) : _data(Vector(vector._data)) {
    }

    void push(const ValueType &value) override { _data.pushBack(value); }
    void pop() override { _data.popBack(); }
    const ValueType &top() const override { return _data[_data.size() - 1]; }
    bool isEmpty() const override { return _data.size() == 0; }
    size_t size() const override { return _data.size(); }
};
