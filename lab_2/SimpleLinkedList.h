#ifndef LAB_2_SIMPLELINKEDLIST_H
#define LAB_2_SIMPLELINKEDLIST_H
using ValueType = double;

#include <iostream>

struct Node {
    ValueType data;
    Node *next;

    Node(ValueType value);
};

class SimpleLinkedList {
    Node *head;
    size_t list_size;

public:
    SimpleLinkedList();

    SimpleLinkedList(const SimpleLinkedList &copyList);

    ~SimpleLinkedList();

    void push_back(ValueType value);

    void pop_back();

    size_t size() const;

    const ValueType& top() const;

    bool find(ValueType value) const;
};
#endif //LAB_2_SIMPLELINKEDLIST_H
