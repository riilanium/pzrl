#include "BST.h"

#include <functional>
#include <iostream>

BinarySearchTree::Node::Node(Key key, Value value,
                             Node *parent, Node *left, Node *right)
    : keyValuePair(key, value), parent(parent), left(left), right(right) {
}

BinarySearchTree::Node::Node(const Node &other)
    : keyValuePair(other.keyValuePair), parent(nullptr), left(nullptr), right(nullptr) {
    if (other.left) {
        left = new Node(*other.left);
        left->parent = this;
    }

    if (other.right) {
        right = new Node(*other.right);
        right->parent = this;
    }
}

bool BinarySearchTree::Node::operator==(const Node &other) const {
    return keyValuePair == other.keyValuePair;
}

void BinarySearchTree::Node::output_node_tree() const {
    if (left) left->output_node_tree();

    std::cout << keyValuePair.first << " : "
            << keyValuePair.second << "\n";

    if (right) right->output_node_tree();
}

void BinarySearchTree::Node::insert(const Key &key, const Value &value) {
    if (key < keyValuePair.first) {
        if (left == nullptr)
            left = new Node(key, value, this);
        else
            left->insert(key, value);
    } else {
        if (right == nullptr)
            right = new Node(key, value, this);
        else
            right->insert(key, value);
    }
}

void BinarySearchTree::Node::erase(const Key &key) {
    if (key < keyValuePair.first) {
        if (left) left->erase(key);
        return;
    }

    if (key > keyValuePair.first) {
        if (right) right->erase(key);
        return;
    }

    if (left && right) {
        Node *successor = right;

        while (successor->left)
            successor = successor->left;

        keyValuePair = successor->keyValuePair;

        right->erase(successor->keyValuePair.first);
    } else if (right) {
        keyValuePair = right->keyValuePair;

        Node *toDelete = right;

        left = toDelete->left;
        right = toDelete->right;

        if (left)
            left->parent = this;

        if (right)
            right->parent = this;

        delete toDelete;
    } else if (left) {
        keyValuePair = left->keyValuePair;

        Node *toDelete = left;

        left = toDelete->left;
        right = toDelete->right;

        if (left)
            left->parent = this;

        if (right)
            right->parent = this;

        delete toDelete;
    } else {
        if (parent) {
            if (parent->left == this)
                parent->left = nullptr;
            else
                parent->right = nullptr;

            delete this;
        }
    }
}

BinarySearchTree::Iterator::Iterator(Node *node)
    : _node(node) {
}

std::pair<Key, Value> &
BinarySearchTree::Iterator::operator*() {
    return _node->keyValuePair;
}

const std::pair<Key, Value> &
BinarySearchTree::Iterator::operator*() const {
    return _node->keyValuePair;
}

std::pair<Key, Value> *
BinarySearchTree::Iterator::operator->() {
    return &_node->keyValuePair;
}

const std::pair<Key, Value> *
BinarySearchTree::Iterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator
BinarySearchTree::Iterator::operator++() {
    if (_node->right) {
        _node = _node->right;

        while (_node->left)
            _node = _node->left;
    } else {
        Node *prev = _node;

        _node = _node->parent;

        while (_node != nullptr && _node->right == prev) {
            prev = _node;
            _node = _node->parent;
        }
    }

    return *this;
}

BinarySearchTree::Iterator
BinarySearchTree::Iterator::operator++(int) {
    Iterator old = *this;

    ++(*this);

    return old;
}

BinarySearchTree::Iterator
BinarySearchTree::Iterator::operator--() {
    if (_node->left) {
        _node = _node->left;

        while (_node->right)
            _node = _node->right;
    } else {
        Node *prev = _node;

        _node = _node->parent;

        while (_node != nullptr && _node->left == prev) {
            prev = _node;
            _node = _node->parent;
        }
    }

    return *this;
}

BinarySearchTree::Iterator
BinarySearchTree::Iterator::operator--(int) {
    Iterator old = *this;

    --(*this);

    return old;
}

bool BinarySearchTree::Iterator::operator==(
    const Iterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(
    const Iterator &other) const {
    return _node != other._node;
}

BinarySearchTree::ConstIterator::ConstIterator(
    const Node *node)
    : _node(node) {
}

const std::pair<Key, Value> &
BinarySearchTree::ConstIterator::operator*() const {
    return _node->keyValuePair;
}

const std::pair<Key, Value> *
BinarySearchTree::ConstIterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator
BinarySearchTree::ConstIterator::operator++() {
    if (_node->right) {
        _node = _node->right;

        while (_node->left)
            _node = _node->left;
    } else {
        const Node *prev = _node;

        _node = _node->parent;

        while (_node != nullptr && _node->right == prev) {
            prev = _node;
            _node = _node->parent;
        }
    }

    return *this;
}

BinarySearchTree::ConstIterator
BinarySearchTree::ConstIterator::operator++(int) {
    ConstIterator old = *this;

    ++(*this);

    return old;
}

BinarySearchTree::ConstIterator
BinarySearchTree::ConstIterator::operator--() {
    if (_node->left) {
        _node = _node->left;

        while (_node->right)
            _node = _node->right;
    } else {
        const Node *prev = _node;

        _node = _node->parent;

        while (_node != nullptr && _node->left == prev) {
            prev = _node;
            _node = _node->parent;
        }
    }

    return *this;
}

BinarySearchTree::ConstIterator
BinarySearchTree::ConstIterator::operator--(int) {
    ConstIterator old = *this;

    --(*this);

    return old;
}

bool BinarySearchTree::ConstIterator::operator==(
    const ConstIterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(
    const ConstIterator &other) const {
    return _node != other._node;
}

BinarySearchTree::BinarySearchTree(
    const BinarySearchTree &other)
    : _size(other._size) {
    _root = other._root
                ? new Node(*other._root)
                : nullptr;
}

BinarySearchTree &
BinarySearchTree::operator=(
    const BinarySearchTree &other) {
    if (this != &other) {
        std::function<void(Node *)> destroy =
                [&](Node *n) {
            if (!n)
                return;

            destroy(n->left);
            destroy(n->right);

            delete n;
        };

        destroy(_root);

        _root = other._root
                    ? new Node(*other._root)
                    : nullptr;

        _size = other._size;
    }

    return *this;
}
