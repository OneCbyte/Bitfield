#pragma once
#include <iostream>

class BitField {
private:
	size_t lenght;
	size_t mem_lenght;
	unsigned int* mem;
    static const int bits_in_int = sizeof(unsigned int) * 8;
public:
    BitField(size_t len = 0);
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