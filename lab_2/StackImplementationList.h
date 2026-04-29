#ifndef STACK_IMPLEMENTATION_LIST_H
#define STACK_IMPLEMENTATION_LIST_H

#include "StackImplementation.h"
#include "SimpleLinkedList.h"

class StackImplementationList : public IStackImplementation {
    SimpleLinkedList _data;

public:
    StackImplementationList() : _data() {
    }

    StackImplementationList(const StackImplementationList &copyList) : _data(copyList._data) {
    }

    void push(const ValueType &value) override {
        _data.push_back(value);
    }

    void pop() override {
        _data.pop_back();
    }

    bool isEmpty() const override {
        return _data.size() == 0;
    }

    const ValueType &top() const override {
        return _data.top();
    }

    size_t size() const override {
        return _data.size();
    }
};

#endif
