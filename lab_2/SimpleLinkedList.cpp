#include <iostream>

#include "SimpleLinkedList.h"

Node::Node(ValueType value) : data(value), next(nullptr) {
}

SimpleLinkedList::SimpleLinkedList() : head(nullptr), list_size(0) {
}

SimpleLinkedList::SimpleLinkedList(const SimpleLinkedList &copyList) : head(nullptr), list_size(copyList.list_size) {
    if (copyList.head == nullptr) {
        return;
    }

    head = new Node(copyList.head->data);
    Node *current = head;
    Node *currentCopy = copyList.head->next;

    while (currentCopy != nullptr) {
        current->next = new Node(currentCopy->data);
        current = current->next;
        currentCopy = currentCopy->next;
    }
}

SimpleLinkedList::~SimpleLinkedList() {
    while (head != nullptr) {
        Node *temp = head;
        head = head->next;
        delete temp;
    }
}


void SimpleLinkedList::push_back(ValueType value) {
    Node *new_node = new Node(value);

    if (head == nullptr) {
        head = new_node;
    } else {
        Node *current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = new_node;
    }
    list_size++;
}

void SimpleLinkedList::pop_back() {
    if (head == nullptr) {
        return;
    }

    if (head->next == nullptr) {
        delete head;
        head = nullptr;
    } else {
        Node *current = head;
        while (current->next->next != nullptr) {
            current = current->next;
        }
        delete current->next;
        current->next = nullptr;
    }
    list_size--;
}

size_t SimpleLinkedList::size() const {
    return list_size;
}

bool SimpleLinkedList::find(ValueType value) const {
    Node *current = head;
    while (current != nullptr) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

const ValueType &SimpleLinkedList::top() const {
    if (list_size == 0) {
        throw std::out_of_range("List is empty");
    }

    Node *current = head;
    while (current->next != nullptr) {
        current = current->next;
    }
    return current->data;
}
