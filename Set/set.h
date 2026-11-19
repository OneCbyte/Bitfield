#pragma once
#include "bitfield.h"
#include <iostream>

class Set {
private:
    size_t max_power;     // Максимальная мощность множества (элементы от 0 до max_power - 1)
    BitField bit_field;   // Битовое поле для хранения элементов (композиция)

public:
    explicit Set(size_t mp = 0);
    Set(const Set& other);
    Set(const BitField& bf); // Конструктор преобразования из битового поля

    // Перегрузка оператора приведения типа к BitField
    operator BitField() const;

    size_t get_max_power() const;

    // Операции над элементами
    void insert(size_t elem);
    void erase(size_t elem);
    bool is_member(size_t elem) const;

    // Операторы сравнения и присваивания
    Set& operator=(const Set& other);
    bool operator==(const Set& other) const;
    bool operator!=(const Set& other) const;

    // Операции с одним элементом (добавление и удаление)
    Set& operator+=(size_t elem);
    Set& operator-=(size_t elem);
    Set operator+(size_t elem) const;
    Set operator-(size_t elem) const;

    // Операции с другим множеством (объединение, разность, пересечение)
    Set& operator+=(const Set& other);
    Set& operator-=(const Set& other);
    Set& operator*=(const Set& other);
    Set operator+(const Set& other) const;
    Set operator-(const Set& other) const;
    Set operator*(const Set& other) const;

    // Унарный минус — дополнение множества (противоположное множество)
    Set operator-() const;

    // Ввод / вывод
    friend std::ostream& operator<<(std::ostream& os, const Set& s);
    friend std::istream& operator>>(std::istream& is, Set& s);
};
