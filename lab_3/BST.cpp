#include "BST.h"

#include <functional>
#include <iostream>

// ============================================================
//  Node — конструкторы и операторы
// ============================================================

BinarySearchTree::Node::Node(Key key, Value value,
                              Node *parent, Node *left, Node *right)
    : keyValuePair(key, value), parent(parent), left(left), right(right) {}

BinarySearchTree::Node::Node(const Node &other)
    : keyValuePair(other.keyValuePair), parent(nullptr), left(nullptr), right(nullptr)
{
    // Рекурсивно копируем левое поддерево
    if (other.left) {
        left = new Node(*other.left);
        left->parent = this;   // восстанавливаем указатель на родителя
    }
    // Рекурсивно копируем правое поддерево
    if (other.right) {
        right = new Node(*other.right);
        right->parent = this;
    }
}

bool BinarySearchTree::Node::operator==(const Node &other) const {
    return keyValuePair == other.keyValuePair;
}

// ============================================================
//  Node::output_node_tree — симметричный (in-order) обход
//  Результат: ключи выводятся в порядке возрастания
// ============================================================

void BinarySearchTree::Node::output_node_tree() const {
    if (left)  left->output_node_tree();          // сначала всё меньшее
    std::cout << keyValuePair.first << " : " << keyValuePair.second << "\n";
    if (right) right->output_node_tree();         // затем всё большее
}

// ============================================================
//  Node::insert — вставка в поддерево с сохранением инварианта
//  Инвариант BST: левый потомок < родитель <= правый потомок
//  Дубликаты (key == текущий ключ) уходят в правое поддерево
// ============================================================

void BinarySearchTree::Node::insert(const Key &key, const Value &value) {
    if (key < keyValuePair.first) {
        // Ключ меньше — идём влево
        if (left == nullptr)
            left = new Node(key, value, this);  // нашли свободное место
        else
            left->insert(key, value);           // продолжаем поиск в левом поддереве
    } else {
        // Ключ >= текущего — идём вправо (дубликаты тоже вправо)
        if (right == nullptr)
            right = new Node(key, value, this);
        else
            right->insert(key, value);
    }
}

// ============================================================
//  Node::erase — удаление ОДНОГО узла с заданным ключом
//
//  Используется «трюк копирования данных»: вместо удаления
//  текущего узла (this) мы копируем в него данные подходящего
//  соседа и удаляем соседа. Благодаря этому указатель
//  BinarySearchTree::_root никогда не становится «висячим»,
//  даже если удаляется корень.
//
//  Три классических случая удаления узла из BST:
//    1. Лист (нет потомков)       — просто отвязываем от родителя
//    2. Один потомок              — «поглощаем» потомка: копируем
//                                   его данные и удаляем сам узел-потомок
//    3. Два потомка               — заменяем данные in-order successor'ом
//                                   (наименьший ключ в правом поддереве)
//                                   и удаляем successor (у него нет левого потомка)
// ============================================================

void BinarySearchTree::Node::erase(const Key &key) {
    if (key < keyValuePair.first) {
        // Ключ меньше — ищем в левом поддереве
        if (left) left->erase(key);
        return;
    }
    if (key > keyValuePair.first) {
        // Ключ больше — ищем в правом поддереве
        if (right) right->erase(key);
        return;
    }

    // ── Нашли узел для удаления ──────────────────────────────────

    if (left && right) {
        // Случай 3: два потомка.
        // Находим in-order successor: делаем один шаг вправо,
        // затем спускаемся как можно левее — это наименьший узел
        // в правом поддереве, следующий за нами в порядке обхода.
        Node *successor = right;
        while (successor->left) successor = successor->left;

        // Копируем данные successor'а в текущий узел —
        // this «превращается» в successor
        keyValuePair = successor->keyValuePair;

        // Удаляем теперь уже ненужный оригинальный successor
        // из правого поддерева (у него гарантированно нет левого потомка)
        right->erase(successor->keyValuePair.first);

    } else if (right) {
        // Случай 2б: только правый потомок.
        // Копируем правого потомка в текущий узел и удаляем потомка.
        keyValuePair = right->keyValuePair;
        Node *toDelete = right;
        // Берём потомков правого потомка себе
        left  = toDelete->left;
        right = toDelete->right;
        if (left)  left->parent  = this;
        if (right) right->parent = this;
        delete toDelete;

    } else if (left) {
        // Случай 2а: только левый потомок.
        // Симметрично: копируем левого потомка в текущий узел.
        keyValuePair = left->keyValuePair;
        Node *toDelete = left;
        left  = toDelete->left;
        right = toDelete->right;
        if (left)  left->parent  = this;
        if (right) right->parent = this;
        delete toDelete;

    } else {
        // Случай 1: лист.
        // Единственный случай, когда нам нужно удалить именно this.
        // Если есть родитель — отвязываемся от него и удаляем себя.
        // Если родителя нет — this является корнем-листом дерева;
        // этот случай обрабатывается выше в BinarySearchTree::erase.
        if (parent) {
            if (parent->left == this)
                parent->left = nullptr;
            else
                parent->right = nullptr;
            delete this;
        }
    }
}

// ============================================================
//  Iterator
// ============================================================

BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {}

std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() {
    return _node->keyValuePair;
}

const std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() const {
    return _node->keyValuePair;
}

std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() {
    return &_node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator
BinarySearchTree::Iterator::operator++() {

    // защита от ++end()
    if (_node == nullptr)
        return *this;

    if (_node->right) {

        _node = _node->right;

        while (_node->left)
            _node = _node->left;
    }
    else {

        Node *prev = _node;
        _node = _node->parent;

        while (_node != nullptr && _node->right == prev) {
            prev = _node;
            _node = _node->parent;
        }
    }

    return *this;
}

// Post-increment: возвращаем старое состояние, затем продвигаемся
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator old = *this;
    ++(*this);
    return old;
}

BinarySearchTree::Iterator
BinarySearchTree::Iterator::operator--() {

    // защита от --end()
    if (_node == nullptr)
        return *this;

    if (_node->left) {

        _node = _node->left;

        while (_node->right)
            _node = _node->right;
    }
    else {

        Node *prev = _node;
        _node = _node->parent;

        while (_node != nullptr && _node->left == prev) {
            prev = _node;
            _node = _node->parent;
        }
    }

    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    Iterator old = *this;
    --(*this);
    return old;
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const {
    return _node != other._node;
}

// ============================================================
//  ConstIterator — полная копия Iterator, но с const-указателем
// ============================================================

BinarySearchTree::ConstIterator::ConstIterator(const Node *node) : _node(node) {}

const std::pair<Key, Value> &BinarySearchTree::ConstIterator::operator*() const {
    return _node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::ConstIterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator
BinarySearchTree::ConstIterator::operator++() {

    // защита от ++cend()
    if (_node == nullptr)
        return *this;

    if (_node->right) {

        _node = _node->right;

        while (_node->left)
            _node = _node->left;
    }
    else {

        const Node *prev = _node;
        _node = _node->parent;

        while (_node != nullptr && _node->right == prev) {
            prev = _node;
            _node = _node->parent;
        }
    }

    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    ConstIterator old = *this;
    ++(*this);
    return old;
}

BinarySearchTree::ConstIterator
BinarySearchTree::ConstIterator::operator--() {

    // защита от --cend()
    if (_node == nullptr)
        return *this;

    if (_node->left) {

        _node = _node->left;

        while (_node->right)
            _node = _node->right;
    }
    else {

        const Node *prev = _node;
        _node = _node->parent;

        while (_node != nullptr && _node->left == prev) {
            prev = _node;
            _node = _node->parent;
        }
    }

    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    ConstIterator old = *this;
    --(*this);
    return old;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const {
    return _node != other._node;
}

// ============================================================
//  BinarySearchTree — управление жизненным циклом
// ============================================================

// Post-order удаление поддерева.
// Реализовано как лямбда внутри методов-членов, поскольку Node —
// приватный тип и недоступен из свободных функций за пределами класса.
// Каждый метод, которому нужно удалить поддерево, вызывает destroy(_root).

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other)
    : _size(other._size)
{
    // Node(const Node&) рекурсивно копирует всё поддерево
    _root = other._root ? new Node(*other._root) : nullptr;
}

BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &other) {
    if (this != &other) {
        // Сначала освобождаем текущее дерево
        std::function<void(Node*)> destroy = [&](Node *n) {
            if (!n) return;
            destroy(n->left);
            destroy(n->right);
            delete n;
        };
        destroy(_root);
        _root = other._root ? new Node(*other._root) : nullptr;
        _size = other._size;
    }
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept
    : _root(other._root), _size(other._size)
{
    // Обнуляем источник, чтобы его деструктор ничего не удалял
    other._root = nullptr;
    other._size = 0;
}

BinarySearchTree &BinarySearchTree::operator=(BinarySearchTree &&other) noexcept {
    if (this != &other) {
        std::function<void(Node*)> destroy = [&](Node *n) {
            if (!n) return;
            destroy(n->left);
            destroy(n->right);
            delete n;
        };
        destroy(_root);
        _root = other._root;
        _size = other._size;
        other._root = nullptr;
        other._size = 0;
    }
    return *this;
}

BinarySearchTree::~BinarySearchTree() {
    // Post-order: сначала потомки, потом сам узел
    std::function<void(Node*)> destroy = [&](Node *n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    };
    destroy(_root);
}

// ============================================================
//  Вставка
// ============================================================

void BinarySearchTree::insert(const Key &key, const Value &value) {
    if (_root == nullptr)
        _root = new Node(key, value);   // первый элемент становится корнем
    else
        _root->insert(key, value);      // делегируем рекурсии узла
    ++_size;
}

// ============================================================
//  Удаление всех узлов с заданным ключом
//
//  Node::erase удаляет по одному узлу за вызов и не может
//  удалить корень-лист (не знает указатель _root).
//  Поэтому BinarySearchTree::erase:
//    1. Обрабатывает случай «корень — лист с нужным ключом» сам,
//    2. Повторяет вызов Node::erase, пока ключ встречается в дереве.
// ============================================================

void BinarySearchTree::erase(const Key &key) {
    if (!_root) return;

    while (true) {
        // Особый случай: корень является листом с нужным ключом.
        // Node::erase не трогает корень-лист (нет родителя),
        // поэтому обрабатываем здесь вручную.
        if (_root->keyValuePair.first == key
            && !_root->left && !_root->right)
        {
            delete _root;
            _root = nullptr;
            --_size;
            return;
        }

        // Проверяем, есть ли ещё хоть один узел с данным ключом
        if (find(key) == end()) break;

        // Удаляем один узел (корень не является листом — безопасно)
        _root->erase(key);
        --_size;
    }
}

// ============================================================
//  Поиск
// ============================================================

// Находим первый (наименьший по in-order) узел с данным ключом.
// Так как дубликаты всегда уходят вправо при вставке,
// первый встреченный при стандартном BST-спуске узел
// является наименьшим по позиции в in-order обходе.
BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
    const Node *cur = _root;
    while (cur) {
        if (key == cur->keyValuePair.first) return ConstIterator(cur);
        cur = (key < cur->keyValuePair.first) ? cur->left : cur->right;
    }
    return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
    Node *cur = _root;
    while (cur) {
        if (key == cur->keyValuePair.first) return Iterator(cur);
        cur = (key < cur->keyValuePair.first) ? cur->left : cur->right;
    }
    return end();
}

// ============================================================
//  equalRange — полуинтервал [first, last) для одного ключа
// ============================================================

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key &key) {
    Iterator first = find(key);
    Iterator last  = first;
    // Двигаемся вперёд, пока ключ совпадает
    while (last != end() && last->first == key)
        ++last;
    return {first, last};
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key &key) const {
    ConstIterator first = find(key);
    ConstIterator last  = first;
    while (last != cend() && last->first == key)
        ++last;
    return {first, last};
}

// ============================================================
//  min / max по всему дереву — крайние узлы in-order обхода
// ============================================================

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
    if (!_root) return cend();
    // Самый левый узел — наименьший ключ
    const Node *cur = _root;
    while (cur->left) cur = cur->left;
    return ConstIterator(cur);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
    if (!_root) return cend();
    // Самый правый узел — наибольший ключ
    const Node *cur = _root;
    while (cur->right) cur = cur->right;
    return ConstIterator(cur);
}

// ============================================================
//  min / max по значению среди узлов с заданным ключом
// ============================================================

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
    auto [first, last] = equalRange(key);
    if (first == last) return cend();   // ключ не найден
    ConstIterator result = first;
    for (auto it = first; it != last; ++it)
        if (it->second < result->second) result = it;
    return result;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
    auto [first, last] = equalRange(key);
    if (first == last) return cend();
    ConstIterator result = first;
    for (auto it = first; it != last; ++it)
        if (it->second > result->second) result = it;
    return result;
}

// ============================================================
//  Итераторы на начало / конец
// ============================================================

BinarySearchTree::Iterator BinarySearchTree::begin() {
    if (!_root) return end();
    Node *cur = _root;
    while (cur->left) cur = cur->left;   // наименьший ключ — самый левый узел
    return Iterator(cur);
}

// end() — «за последним элементом»; представляем как nullptr
BinarySearchTree::Iterator BinarySearchTree::end() {
    return Iterator(nullptr);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    if (!_root) return cend();
    const Node *cur = _root;
    while (cur->left) cur = cur->left;
    return ConstIterator(cur);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    return ConstIterator(nullptr);
}

// ============================================================
//  Вспомогательные
// ============================================================

size_t BinarySearchTree::size() const {
    return _size;
}

void BinarySearchTree::output_tree() {
    if (_root)
        _root->output_node_tree();
    else
        std::cout << "(дерево пусто)\n";
}
