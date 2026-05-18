#include "BST.h"
#include <iostream>

using namespace std;

BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left,
                             Node *right)
{
  keyValuePair.first = key;
  keyValuePair.second = value;
  this->parent = parent;
  this->left = left;
  this->right = right;
}

BinarySearchTree::Node::Node(const Node &other)
{
  keyValuePair.first = other.keyValuePair.first;
  keyValuePair.second = other.keyValuePair.second;
  if (other.left)
  {
    left = new Node(*other.left);
    left->parent = this;
  }
  if (other.right)
  {
    right = new Node(*other.right);
    right->parent = this;
  }
}

bool BinarySearchTree::Node::operator==(const Node &other) const
{
  return (parent == other.parent && left == other.left && right == other.right);
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other)
{
  if (other._root)
  {
    _root = new Node(*other._root);
  }
  _size = other._size;
}

BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &other)
{
  if (this != &other)
  {
    if (other._root)
    {
      cleartree(_root);
      _root = new Node(*other._root);
    }
    _size = other._size;
  }
  return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept
{
  _root = other._root;
  _size = other._size;

  other._root = nullptr;
  other._size = 0;
}

BinarySearchTree &
BinarySearchTree::operator=(BinarySearchTree &&other) noexcept
{
  if (this != &other)
  {
    _root = other._root;
    _size = other._size;

    other._root = nullptr;
    other._size = 0;
  }
  return *this;
}

BinarySearchTree::~BinarySearchTree()
{
  if (_root)
  {
    cleartree(_root);
  }
}

void BinarySearchTree::cleartree(Node *node)
{
  if (node)
  {
    if (node->left)
    {
      cleartree(node->left);
    }
    if (node->right)
    {
      cleartree(node->right);
    }
    delete node;
  }
}

BinarySearchTree::Iterator::Iterator(Node *node)
{
  _node = node;
}

std::pair<Key, Value> &BinarySearchTree::Iterator::operator*()
{
  if (_node == nullptr) throw std::out_of_range("Dereferencing end() iterator");
  return _node->keyValuePair;
}

const std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() const
{
  if (_node == nullptr) throw std::out_of_range("Dereferencing end() iterator");
  return _node->keyValuePair;
}

std::pair<Key, Value> *BinarySearchTree::Iterator::operator->()
{
  if (_node == nullptr) throw std::out_of_range("Dereferencing end() iterator");
  return &(_node->keyValuePair);
}

const std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() const
{
  if (_node == nullptr) throw std::out_of_range("Dereferencing end() iterator");
  return &(_node->keyValuePair);
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++()
{
  if (_node == nullptr) return *this;
  if (_node->right != nullptr)
  {
    _node = _node->right;
    while (_node->left != nullptr)
    {
      _node = _node->left;
    }
  }
  else
  {
    Node *parent = _node->parent;
    while (parent != nullptr && _node == parent->right)
    {
      _node = parent;
      parent = parent->parent;
    }
    _node = parent;
  }
  return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int)
{
  Iterator temp = *this;
  ++(*this);
  return temp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--()
{
  if (_node == nullptr) return *this;
  if (_node->left != nullptr)
  {
    _node = _node->left;
    while (_node->right != nullptr)
    {
      _node = _node->right;
    }
  }
  else
  {
    Node *parent = _node->parent;
    while (parent != nullptr && _node == parent->left)
    {
      _node = parent;
      parent = parent->parent;
    }
    _node = parent;
  }
  return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int)
{
  Iterator temp = *this;
  --(*this);
  return temp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const
{
  return (_node == other._node);
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const
{
  return (_node != other._node);
}

BinarySearchTree::ConstIterator::ConstIterator(const Node *node)
{
  _node = node;
}

const std::pair<Key, Value> &
BinarySearchTree::ConstIterator::operator*() const
{
  if (_node == nullptr) throw std::out_of_range("Dereferencing end() iterator");
  return _node->keyValuePair;
}

const std::pair<Key, Value> *
BinarySearchTree::ConstIterator::operator->() const
{
  if (_node == nullptr) throw std::out_of_range("Dereferencing end() iterator");
  return &(_node->keyValuePair);
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
  if (_node == nullptr) return *this;
  if (_node->right != nullptr)
  {
    _node = _node->right;
    while (_node->left != nullptr)
    {
      _node = _node->left;
    }
  }
  else
  {
    Node *parent = _node->parent;
    while (parent != nullptr && _node == parent->right)
    {
      _node = parent;
      parent = parent->parent;
    }
    _node = parent;
  }
  return *this;
}

BinarySearchTree::ConstIterator
BinarySearchTree::ConstIterator::operator++(int)
{
  ConstIterator temp = *this;
  ++(*this);
  return temp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--()
{
  if (_node == nullptr) return *this;
  if (_node->left != nullptr)
  {
    _node = _node->left;
    while (_node->right != nullptr)
    {
      _node = _node->right;
    }
  }
  else
  {
    Node *parent = _node->parent;
    while (parent != nullptr && _node == parent->left)
    {
      _node = parent;
      parent = parent->parent;
    }
    _node = parent;
  }
  return *this;
}

BinarySearchTree::ConstIterator
BinarySearchTree::ConstIterator::operator--(int)
{
  ConstIterator temp = *this;
  --(*this);
  return temp;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const
{
  return (_node == other._node);
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const
{
  return (_node != other._node);
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const
{
  Node *current = _root;
  while (current != nullptr)
  {
    if (key > current->keyValuePair.first)
    {
      current = current->right;
    }
    else if (key < current->keyValuePair.first)
    {
      current = current->left;
    }
    else if (key == current->keyValuePair.first)
    {
      return ConstIterator(current);
    }
    else
    {
      return cend();
    }
  }
  return ConstIterator(nullptr);
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key)
{
  Node *current = _root;
  while (current != nullptr)
  {
    if (current->right && key > current->keyValuePair.first)
    {
      current = current->right;
    }
    else if (current->left && key < current->keyValuePair.first)
    {
      current = current->left;
    }
    else if (key == current->keyValuePair.first)
    {
      return Iterator(current);
    }
    else
    {
      return end();
    }
  }
  return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key &key)
{
  Iterator first = end();
  Iterator last = end();

  for (auto it = begin(); it != end(); ++it)
  {
    if ((*it).first == key)
    {
      first = it;
      ++it;
      while (it != end() && (*it).first == key)
      {
        ++it;
      }
      last = it;
      break;
    }
    if ((*it).first > key) break;
  }

  return std::make_pair(first, last);
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key &key) const
{
    ConstIterator first = cend();
    ConstIterator last = cend();

    for (auto it = cbegin(); it != cend(); ++it)
    {
        if ((*it).first == key)
        {
            first = it;
            break;
        }
        else if ((*it).first > key)
        {
            return std::make_pair(first, first);
        }
    }

    if (first != cend())
    {
        for (auto it = first; it != cend(); ++it)
        {
            if ((*it).first != key)
            {
                last = it;
                break;
            }
        }
    }

    return std::make_pair(first, last);
}
BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const
{
    Node* result = nullptr;
    findMinRecursive(_root, key, result);
    return ConstIterator(result);
}

void BinarySearchTree::findMinRecursive(Node* node, const Key& key, Node*& result) const
{
    if (!node) return;

    findMinRecursive(node->left, key, result);

    if (node->keyValuePair.first == key)
    {
        if (!result || node->keyValuePair.second < result->keyValuePair.second)
        {
            result = node;
        }
    }

    findMinRecursive(node->right, key, result);
}


BinarySearchTree::ConstIterator BinarySearchTree::min() const
{
  Node *current = _root;
  while (current->left)
  {
    current = current->left;
  }
  return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const
{
    Node* result = nullptr;
    findMaxRecursive(_root, key, result);
    return ConstIterator(result);
}

void BinarySearchTree::findMaxRecursive(Node* node, const Key& key, Node*& result) const
{
    if (!node) return;

    if (node->keyValuePair.first == 0 && node->keyValuePair.second == 0)
        return;

    findMaxRecursive(node->left, key, result);

    if (node->keyValuePair.first == key)
    {
        if (!result || node->keyValuePair.second > result->keyValuePair.second)
        {
            result = node;
        }
    }

    findMaxRecursive(node->right, key, result);
}


BinarySearchTree::ConstIterator BinarySearchTree::max() const
{
  Node *current = _root;
  while (current && current->right)
  {
    current = current->right;
  }
  return ConstIterator(current);
}

BinarySearchTree::Iterator BinarySearchTree::begin()
{
  Node *current = _root;
  while (current && current->left)
  {
    current = current->left;
  }
  return Iterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const
{
  return min();
}

BinarySearchTree::Iterator BinarySearchTree::end()
{
  return Iterator(nullptr);
}


BinarySearchTree::ConstIterator BinarySearchTree::cend() const
{
  return ConstIterator(nullptr);
}

size_t BinarySearchTree::size() const
{
  return _size;
}


void BinarySearchTree::Node:: output_node_tree() const
{
    if (left)
    {
        left->output_node_tree();
    }
    std::cout<<"["<<keyValuePair.first<<" "<<keyValuePair.second<<"]"<<std::endl;
    if (right)
    {
        right->output_node_tree();
    }
}

void BinarySearchTree::output_tree()
{
  if (_root != nullptr)
  {
    _root->output_node_tree();
  }
  else
  {
    std::cout << "(Empty tree)" << std::endl;
  }
}

void BinarySearchTree::Node::insert(const Key &key, const Value &value)
{
  if (key < keyValuePair.first)
  {
    if (left)
    {
      left->insert(key, value);
    } else
    {
      Node *node = new Node(key, value, this, nullptr, nullptr);
      this->left = node;
    }
  }
  if (key >= keyValuePair.first)
  {
    if (right)
    {
      right->insert(key, value);
    } else
    {
      Node *node = new Node(key, value, this, nullptr, nullptr);
      this->right = node;
    }
  }
}

void BinarySearchTree::insert(const Key &key, const Value &value)
{
  if (_root)
  {
    _root->insert(key, value);
  }
  else
  {
    _root = new Node(key, value);
  }
  _size++;
}

BinarySearchTree::Node *BinarySearchTree::Node::erase_node(Node *node, const Key &key)
{
  if (node == nullptr)
  {
    return node;
  }
  if (key < node->keyValuePair.first)
  {
    node->left = erase_node(node->left, key);
    return node;
  }
  else if (key > node->keyValuePair.first)
  {
    node->right = erase_node(node->right, key);
    return node;
  }

  if (node->left == nullptr)
  {
    Node *temp = node->right;
    delete node;
    return temp;
  }
  else if (node->right == nullptr)
  {
    Node *temp = node->left;
    delete node;
    return temp;
  }
  Node *temp = node->right;
  Node *parent = node;
  while (temp->left != nullptr)
  {
    parent = temp;
    temp = temp->left;
  }

  node->keyValuePair = temp->keyValuePair;
  if (parent->left == temp)
  {
    parent->left = temp->right;
  }
  else
  {
    parent->right = temp->right;
  }
  delete temp;
  return node;
}

void BinarySearchTree::erase(const Key &key)
{
  while (find(key) != end())
  {
    _root = _root->erase_node(_root, key);
    _size--;
  }

  // Пока в дереве есть ключи, удаляем их
  Iterator f = find(key);
  if (f != end())
  {
    erase(key);
  }
}
void BinarySearchTree::delete_nill()
{
  Node *current = _root;
  if (!current)
    return;
  while (current->right)
  {
    current = current->right;
  }
  if (current != nullptr)
  {
    current->parent->right = nullptr;
    delete current;
  }
}

void BinarySearchTree::insert_nill()
{
  if (_root)
  {
    Node *nil = new Node(0, 0);
    Node *current = _root;
    while (current->right)
    {
      current = current->right;
    }
    if (current!=nullptr)
    {
      current->right = nil;
      nil->parent = current;
    }
  }
}