#pragma once

#include <cstddef>
#include <iostream>

class BitField {
private:
    size_t bit_len;        // Количество битов
    size_t mem_len;        // Количество элементов unsigned int
    unsigned int* p_mem;   // Динамический массив для хранения битов

    static constexpr size_t BITS_PER_INT = sizeof(unsigned int) * 8;

    size_t get_mem_index(size_t pos) const;
    unsigned int get_mem_mask(size_t pos) const;
    void clear_unused_bits(); // Обнуляет неиспользуемые старшие биты последнего слова

public:
    explicit BitField(size_t len = 0);
    BitField(const BitField& other);
    BitField(BitField&& other) noexcept;
    ~BitField();

    BitField& operator=(const BitField& other);
    BitField& operator=(BitField&& other) noexcept;

    size_t get_length() const noexcept;

    // Операции над битами
    void set(size_t pos);
    void reset(size_t pos);
    bool get(size_t pos) const;

    // Сравнение
    bool operator==(const BitField& other) const;
    bool operator!=(const BitField& other) const;

    // Побитовые операторы и версии с присваиванием
    BitField operator~() const;
    BitField operator&(const BitField& other) const;
    BitField operator|(const BitField& other) const;
    BitField& operator&=(const BitField& other);
    BitField& operator|=(const BitField& other);

    // Потоковый ввод/вывод (формат: 01110101...)
    friend std::ostream& operator<<(std::ostream& os, const BitField& bf);
    friend std::istream& operator>>(std::istream& is, BitField& bf);
};
