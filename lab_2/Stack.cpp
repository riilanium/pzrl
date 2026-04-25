#include "Stack.h"

#include <iostream>

Stack::Stack(StackContainer container) {
    _containerType = container;
    if (container == StackContainer::List) {
        _pimpl = new StackImplementationList();
    } else {
        _pimpl = new StackImplementationVector();
    }
}

Stack::Stack(const ValueType *valueArray, const size_t arraySize, StackContainer container) : Stack(container) {
    for (int i = 0; i < arraySize; i++) {
        _pimpl->push(valueArray[i]);
    }
}

Stack::Stack(const Stack &copyStack) : Stack(copyStack._containerType) {
    if (_containerType == StackContainer::List) {
        _pimpl = new StackImplementationList(*dynamic_cast<StackImplementationList *>(copyStack._pimpl));
    } else {
        _pimpl = new StackImplementationVector(*dynamic_cast<StackImplementationVector *>(copyStack._pimpl));
    }
}

Stack &Stack::operator=(const Stack &copyStack) {
    if (this != &copyStack) {
        Stack temp(copyStack);
        std::swap(_pimpl, temp._pimpl);
        std::swap(_containerType, temp._containerType);
    }
    return *this;
}

Stack::Stack(Stack &&moveStack) noexcept
    : _containerType(moveStack._containerType) {
    _pimpl = moveStack._pimpl;
    moveStack._pimpl = nullptr;
}

Stack &Stack::operator=(Stack &&moveStack) noexcept {
    if (this != &moveStack) {
        delete _pimpl;
        _pimpl = moveStack._pimpl;
        _containerType = moveStack._containerType;
        moveStack._pimpl = nullptr;
    }
    return *this;
}

Stack::~Stack() {
    delete _pimpl;
}

void Stack::push(const ValueType &value) {
    _pimpl->push(value);
}

void Stack::pop() {
    _pimpl->pop();
}

const ValueType &Stack::top() const {
    return _pimpl->top();
}

bool Stack::isEmpty() const {
    return _pimpl->isEmpty();
}

size_t Stack::size() const {
    return _pimpl->size();
}
