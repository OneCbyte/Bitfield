#include "pch.h"
#include "set.h"
#include <stdexcept>

Set::Set(size_t mp) : max_power(mp), bit_field(mp) {}

Set::Set(const Set& other) : max_power(other.max_power), bit_field(other.bit_field) {}

Set::Set(const BitField& bf) : max_power(bf.get_length()), bit_field(bf) {}

// Приведение типа Set к BitField
Set::operator BitField() const {
    return bit_field;
}

size_t Set::get_max_power() const {
    return max_power;
}

// Добавить элемент в множество
void Set::insert(size_t elem) {
    if (elem >= max_power) {
        throw std::out_of_range("Элемент выходит за пределы универсума");
    }
    bit_field.set(elem);
}

// Удалить элемент из множества
void Set::erase(size_t elem) {
    if (elem >= max_power) {
        throw std::out_of_range("Элемент выходит за пределы универсума");
    }
    bit_field.reset(elem);
}

// Проверить принадлежность элемента множеству
bool Set::is_member(size_t elem) const {
    if (elem >= max_power) {
        throw std::out_of_range("Элемент выходит за пределы универсума");
    }
    return bit_field.get(elem);
}

Set& Set::operator=(const Set& other) {
    if (this == &other) {
        return *this;
    }
    max_power = other.max_power;
    bit_field = other.bit_field;
    return *this;
}

bool Set::operator==(const Set& other) const {
    return (max_power == other.max_power) && (bit_field == other.bit_field);
}

bool Set::operator!=(const Set& other) const {
    return !(*this == other);
}

// --- Операции с одним элементом ---

Set& Set::operator+=(size_t elem) {
    insert(elem);
    return *this;
}

Set& Set::operator-=(size_t elem) {
    erase(elem);
    return *this;
}

Set Set::operator+(size_t elem) const {
    Set res(*this);
    res.insert(elem);
    return res;
}

Set Set::operator-(size_t elem) const {
    Set res(*this);
    res.erase(elem);
    return res;
}

// --- Операции с другим множеством ---

// Объединение множеств (A | B)
Set& Set::operator+=(const Set& other) {
    if (max_power != other.max_power) {
        throw std::invalid_argument("Множества имеют разную мощность универсума");
    }
    bit_field |= other.bit_field;
    return *this;
}

// Разность множеств: элементы из A, которых нет в B (A & ~B)
Set& Set::operator-=(const Set& other) {
    if (max_power != other.max_power) {
        throw std::invalid_argument("Множества имеют разную мощность универсума");
    }
    bit_field &= (~other.bit_field);
    return *this;
}

// Пересечение множеств (A & B)
Set& Set::operator*=(const Set& other) {
    if (max_power != other.max_power) {
        throw std::invalid_argument("Множества имеют разную мощность универсума");
    }
    bit_field &= other.bit_field;
    return *this;
}

Set Set::operator+(const Set& other) const {
    Set res(*this);
    res += other;
    return res;
}

Set Set::operator-(const Set& other) const {
    Set res(*this);
    res -= other;
    return res;
}

Set Set::operator*(const Set& other) const {
    Set res(*this);
    res *= other;
    return res;
}

// Унарный минус — построение дополнения (~A)
Set Set::operator-() const {
    Set res(~bit_field);
    return res;
}

// Вывод множества в формате { 1 3 5 }
std::ostream& operator<<(std::ostream& os, const Set& s) {
    os << "{ ";
    for (size_t i = 0; i < s.max_power; i++) {
        if (s.is_member(i)) {
            os << i << " ";
        }
    }
    os << "}";
    return os;
}

// Ввод элементов множества до числа -1 (или конца потока)
std::istream& operator>>(std::istream& is, Set& s) {
    int elem;
    while (is >> elem) {
        if (elem < 0) {
            break;
        }
        if (static_cast<size_t>(elem) < s.max_power) {
            s.insert(static_cast<size_t>(elem));
        }
    }
    return is;
}
