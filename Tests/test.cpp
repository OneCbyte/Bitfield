#include "pch.h"
#include <gtest/gtest.h>
#include "bitfield.h"
#include "set.h"

// ==========================================
// Тесты для класса BitField
// ==========================================

TEST(BitFieldTest, CanCreateWithLength) {
    BitField bf(15);
    EXPECT_EQ(bf.get_length(), 15);
    for (size_t i = 0; i < 15; i++) {
        EXPECT_FALSE(bf.get(i));
    }
}

TEST(BitFieldTest, SetResetAndGetBit) {
    BitField bf(40); // Проверяем переход через границу 32 бит
    bf.set(0);
    bf.set(35);

    EXPECT_TRUE(bf.get(0));
    EXPECT_TRUE(bf.get(35));
    EXPECT_FALSE(bf.get(10));

    bf.reset(35);
    EXPECT_FALSE(bf.get(35));
}

TEST(BitFieldTest, ThrowsWhenOutOfRange) {
    BitField bf(10);
    EXPECT_THROW(bf.set(10), std::out_of_range);
    EXPECT_THROW(bf.reset(12), std::out_of_range);
    EXPECT_THROW(bf.get(100), std::out_of_range);
}

TEST(BitFieldTest, CopyAndAssignmentWorkCorrectly) {
    BitField bf1(10);
    bf1.set(2);
    bf1.set(7);

    BitField bf2(bf1);
    EXPECT_EQ(bf1, bf2);

    // Проверяем глубокое копирование (изменение bf2 не меняет bf1)
    bf2.set(5);
    EXPECT_NE(bf1, bf2);
    EXPECT_FALSE(bf1.get(5));

    BitField bf3(3);
    bf3 = bf1;
    EXPECT_EQ(bf1, bf3);
}

TEST(BitFieldTest, BitwiseOperations) {
    BitField a(5), b(5);
    a.set(0); a.set(1); // 11000
    b.set(1); b.set(2); // 01100

    // Проверка И (&)
    BitField res_and = a & b; // 01000
    EXPECT_FALSE(res_and.get(0));
    EXPECT_TRUE(res_and.get(1));
    EXPECT_FALSE(res_and.get(2));

    // Проверка ИЛИ (|)
    BitField res_or = a | b; // 11100
    EXPECT_TRUE(res_or.get(0));
    EXPECT_TRUE(res_or.get(1));
    EXPECT_TRUE(res_or.get(2));
    EXPECT_FALSE(res_or.get(3));

    // Проверка инверсии (~) и очистки лишних битов
    BitField inv_a = ~a; // 00111
    BitField expected(5);
    expected.set(2);
    expected.set(3);
    expected.set(4);
    EXPECT_EQ(inv_a, expected);
}

TEST(BitFieldTest, ThrowsOnDifferentLengthsInBitwiseOps) {
    BitField a(5), b(10);
    EXPECT_THROW(a & b, std::invalid_argument);
    EXPECT_THROW(a | b, std::invalid_argument);
}

// ==========================================
// Тесты для класса Set
// ==========================================

TEST(SetTest, InsertEraseAndIsMember) {
    Set s(10);
    s.insert(3);
    s += 7;

    EXPECT_TRUE(s.is_member(3));
    EXPECT_TRUE(s.is_member(7));
    EXPECT_FALSE(s.is_member(4));

    s.erase(3);
    s -= 7;
    EXPECT_FALSE(s.is_member(3));
    EXPECT_FALSE(s.is_member(7));
}

TEST(SetTest, SetOperationsUnionIntersectionDifference) {
    Set a(6), b(6);
    a = a + 1 + 2 + 3; // {1, 2, 3}
    b = b + 2 + 3 + 4; // {2, 3, 4}

    // Объединение (+)
    Set u = a + b;
    Set expected_u(6);
    expected_u = expected_u + 1 + 2 + 3 + 4;
    EXPECT_EQ(u, expected_u);

    // Пересечение (*)
    Set inter = a * b;
    Set expected_inter(6);
    expected_inter = expected_inter + 2 + 3;
    EXPECT_EQ(inter, expected_inter);

    // Разность (-)
    Set diff = a - b;
    Set expected_diff(6);
    expected_diff = expected_diff + 1;
    EXPECT_EQ(diff, expected_diff);
}

TEST(SetTest, UnaryMinusComplement) {
    Set a(5);
    a.insert(1);
    a.insert(3); // {1, 3}

    Set comp = -a; // {0, 2, 4}
    EXPECT_TRUE(comp.is_member(0));
    EXPECT_FALSE(comp.is_member(1));
    EXPECT_TRUE(comp.is_member(2));
    EXPECT_FALSE(comp.is_member(3));
    EXPECT_TRUE(comp.is_member(4));
}

TEST(SetTest, ConversionsBetweenSetAndBitField) {
    Set s(8);
    s.insert(2);
    s.insert(6);

    // Приведение Set -> BitField
    BitField bf = static_cast<BitField>(s);
    EXPECT_EQ(bf.get_length(), 8);
    EXPECT_TRUE(bf.get(2));
    EXPECT_TRUE(bf.get(6));
    EXPECT_FALSE(bf.get(0));

    // Конструктор Set(BitField)
    Set s_from_bf(bf);
    EXPECT_EQ(s, s_from_bf);
}

TEST(SetTest, SieveOfEratosthenesCheck) {
    size_t n = 20;
    Set primes(n + 1);
    for (size_t i = 2; i <= n; i++) {
        primes.insert(i);
    }

    for (size_t p = 2; p * p <= n; p++) {
        if (primes.is_member(p)) {
            for (size_t k = p * p; k <= n; k += p) {
                primes.erase(k);
            }
        }
    }

    // Простые числа до 20: 2, 3, 5, 7, 11, 13, 17, 19
    Set expected(n + 1);
    expected = expected + 2 + 3 + 5 + 7 + 11 + 13 + 17 + 19;
    EXPECT_EQ(primes, expected);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
