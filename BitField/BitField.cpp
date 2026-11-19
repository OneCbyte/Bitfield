#include "pch.h"
#include "bitfield.h"
#include <stdexcept>
#include <string>

// Считаем, в какой ячейке массива p_mem лежит нужный бит
size_t BitField::get_mem_index(size_t pos) const {
    return pos / BITS_PER_INT;
}

// Создаем битовую маску: единица сдвигается на нужную позицию внутри числа
unsigned int BitField::get_mem_mask(size_t pos) const {
    return 1U << (pos % BITS_PER_INT);
}

// Очищаем мусорные биты в последнем элементе массива (нужно после инверсии ~)
void BitField::clear_unused_bits() {
    if (bit_len == 0 || mem_len == 0) {
        return;
    }
    size_t extra_bits = bit_len % BITS_PER_INT;
    if (extra_bits != 0) {
        unsigned int mask = (1U << extra_bits) - 1U;
        p_mem[mem_len - 1] = p_mem[mem_len - 1] & mask;
    }
}

// Конструктор с параметром (размер битового поля)
BitField::BitField(size_t len) {
    bit_len = len;
    mem_len = (bit_len + BITS_PER_INT - 1) / BITS_PER_INT;

    if (mem_len > 0) {
        p_mem = new unsigned int[mem_len];
        for (size_t i = 0; i < mem_len; i++) {
            p_mem[i] = 0;
        }
    }
    else {
        p_mem = nullptr;
    }
}

// Конструктор копирования (делаем глубокую копию массива)
BitField::BitField(const BitField& other) {
    bit_len = other.bit_len;
    mem_len = other.mem_len;

    if (mem_len > 0) {
        p_mem = new unsigned int[mem_len];
        for (size_t i = 0; i < mem_len; i++) {
            p_mem[i] = other.p_mem[i];
        }
    }
    else {
        p_mem = nullptr;
    }
}

// Конструктор перемещения (забираем указатель себе, у старого объекта обнуляем)
BitField::BitField(BitField&& other) noexcept {
    bit_len = other.bit_len;
    mem_len = other.mem_len;
    p_mem = other.p_mem;

    other.bit_len = 0;
    other.mem_len = 0;
    other.p_mem = nullptr;
}

// Деструктор: освобождаем динамическую память
BitField::~BitField() {
    if (p_mem != nullptr) {
        delete[] p_mem;
    }
}

// Оператор присваивания (копированием)
BitField& BitField::operator=(const BitField& other) {
    if (this == &other) {
        return *this;
    }

    if (mem_len != other.mem_len) {
        delete[] p_mem;
        mem_len = other.mem_len;
        if (mem_len > 0) {
            p_mem = new unsigned int[mem_len];
        }
        else {
            p_mem = nullptr;
        }
    }

    bit_len = other.bit_len;
    for (size_t i = 0; i < mem_len; i++) {
        p_mem[i] = other.p_mem[i];
    }

    return *this;
}

// Оператор присваивания (перемещением)
BitField& BitField::operator=(BitField&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    delete[] p_mem;

    bit_len = other.bit_len;
    mem_len = other.mem_len;
    p_mem = other.p_mem;

    other.bit_len = 0;
    other.mem_len = 0;
    other.p_mem = nullptr;

    return *this;
}

// Узнать длину битового поля
size_t BitField::get_length() const noexcept {
    return bit_len;
}

// Установить бит в 1
void BitField::set(size_t pos) {
    if (pos >= bit_len) {
        throw std::out_of_range("Индекс бита вне диапазона");
    }
    size_t idx = get_mem_index(pos);
    unsigned int mask = get_mem_mask(pos);
    p_mem[idx] = p_mem[idx] | mask;
}

// Сбросить бит в 0
void BitField::reset(size_t pos) {
    if (pos >= bit_len) {
        throw std::out_of_range("Индекс бита вне диапазона");
    }
    size_t idx = get_mem_index(pos);
    unsigned int mask = get_mem_mask(pos);
    p_mem[idx] = p_mem[idx] & (~mask);
}

// Получить значение бита
bool BitField::get(size_t pos) const {
    if (pos >= bit_len) {
        throw std::out_of_range("Индекс бита вне диапазона");
    }
    size_t idx = get_mem_index(pos);
    unsigned int mask = get_mem_mask(pos);

    if ((p_mem[idx] & mask) != 0) {
        return true;
    }
    return false;
}

// Сравнение на равенство
bool BitField::operator==(const BitField& other) const {
    if (bit_len != other.bit_len) {
        return false;
    }
    for (size_t i = 0; i < mem_len; i++) {
        if (p_mem[i] != other.p_mem[i]) {
            return false;
        }
    }
    return true;
}

// Сравнение на неравенство
bool BitField::operator!=(const BitField& other) const {
    return !(*this == other);
}

// Побитовое НЕ (инверсия)
BitField BitField::operator~() const {
    BitField res(bit_len);
    for (size_t i = 0; i < mem_len; i++) {
        res.p_mem[i] = ~p_mem[i];
    }
    res.clear_unused_bits();
    return res;
}

// Побитовое И с присваиванием
BitField& BitField::operator&=(const BitField& other) {
    if (bit_len != other.bit_len) {
        throw std::invalid_argument("Разные длины битовых полей");
    }
    for (size_t i = 0; i < mem_len; i++) {
        p_mem[i] = p_mem[i] & other.p_mem[i];
    }
    return *this;
}

// Побитовое ИЛИ с присваиванием
BitField& BitField::operator|=(const BitField& other) {
    if (bit_len != other.bit_len) {
        throw std::invalid_argument("Разные длины битовых полей");
    }
    for (size_t i = 0; i < mem_len; i++) {
        p_mem[i] = p_mem[i] | other.p_mem[i];
    }
    return *this;
}

// Побитовое И
BitField BitField::operator&(const BitField& other) const {
    BitField res(*this);
    res &= other;
    return res;
}

// Побитовое ИЛИ
BitField BitField::operator|(const BitField& other) const {
    BitField res(*this);
    res |= other;
    return res;
}

// Вывод в поток (01110101...)
std::ostream& operator<<(std::ostream& os, const BitField& bf) {
    for (size_t i = 0; i < bf.bit_len; i++) {
        if (bf.get(i)) {
            os << '1';
        }
        else {
            os << '0';
        }
    }
    return os;
}

// Ввод из потока
std::istream& operator>>(std::istream& is, BitField& bf) {
    std::string str;
    is >> str;
    if (!is) {
        return is;
    }

    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] != '0' && str[i] != '1') {
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    BitField temp(str.size());
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == '1') {
            temp.set(i);
        }
    }
    bf = temp;
    return is;
}
