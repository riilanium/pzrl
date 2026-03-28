#pragma once

#include <cstdlib>

using ValueType = double;

class Vector {
public:
    /*!
        \note Все конструкторы и операторы присваивания,
		кроме конструктора и оператора присваивания перемещением
		должны создавать вектор, где _capacity равно _size
    */

    Vector() = default;

    Vector(const ValueType *rawArray, const size_t size, float coef = 2.0f);

    explicit Vector(const Vector &other);

    Vector &operator=(const Vector &other);

    explicit Vector(Vector &&other) noexcept;

    Vector &operator=(Vector &&other) noexcept;

    ~Vector();


    void pushBack(const ValueType &value);

    void pushFront(const ValueType &value);

    void insert(const ValueType &value, size_t pos);

    void insert(const ValueType *values, size_t size, size_t pos);

    void insert(const Vector &vector, size_t pos);


    void popBack();

    void popFront();

    /*!
        \brief Удалить элемент(ы) из вектора
        \param pos: позиция начала удаляемого участка
        \param count: количество удаляемых элементов
        если (pos + count) > size, то элементы интервала [pos, size) должны быть удалены
    */
    void erase(size_t pos, size_t count = 1);

    /*!
        \brief Удалить элемент(ы) из вектора
        \param beginPos: позиция начала удаляемого участка
        \param endPos: позиция окончания удаляемого участка
        Все элементы интервала [beginPos, endPos) должны быть удалены:
            [1, 2, 3, 4] -> erase(1, 3) -> [1, 4]
        если endPos > size, то элементы интервала [beginPos, size) должны быть удалены
    */
    void eraseBetween(size_t beginPos, size_t endPos);

    //! Количество элементов
    size_t size() const;

    //! Максимальное количество элементов
    size_t capacity() const;

    //! Фактор загруженности
    double loadFactor() const;


    ValueType &operator[](size_t idx);

    const ValueType &operator[](size_t idx) const;


    long long find(const ValueType &value) const;


    void reserve(size_t capacity);

    void shrinkToFit();

    //! Класс, реализующий итератор
    class Iterator {
        ValueType *_ptr;

    public:
        explicit Iterator(ValueType *ptr);

        ValueType &operator*();

        const ValueType &operator*() const;

        ValueType *operator->();

        const ValueType *operator->() const;

        Iterator operator++();

        Iterator operator++(int);

        bool operator==(const Iterator &other) const;

        bool operator!=(const Iterator &other) const;
    };

    //! Получить итератор на начало вектора
    Iterator begin();

    //! Получить итератор на элемент вектора, следующий за последним
    Iterator end();

private:
    ValueType *_data = nullptr; //! "сырой" указатель на данные
    size_t _size = 0; //! текущий размер
    size_t _capacity = 0; //! максимальный размер (количество элементов, под которое выделена память)
    float _multiplicativeCoef = 2.0f; //! коэффициент увеличения _capacity вектора при _size == _capacity
};
