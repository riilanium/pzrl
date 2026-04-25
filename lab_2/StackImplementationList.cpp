#include "StackImplementation.h"
#include "SimpleLinkedList.cpp"

class StackImplementationList : public IStackImplementation {
    SimpleLinkedList _data;

public:
    StackImplementationList() {
        _data = SimpleLinkedList();
    }

    StackImplementationList(const StackImplementationList& copyList) : _data(SimpleLinkedList(copyList._data)) {
    }

    void push(const ValueType &value) override { return _data.push_back(value); }
    void pop() override { _data.pop_back(); }
    bool isEmpty() const override { return _data.size() == 0; }
    const ValueType &top() const override { return _data.top(); }
    size_t size() const override { return _data.size(); }
};
