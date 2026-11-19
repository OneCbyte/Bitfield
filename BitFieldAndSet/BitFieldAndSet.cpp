#include <iostream>
#include "set.h"

int main() {
    setlocale(LC_ALL, "Russian");

    size_t n;
    std::cout << "Введите верхнюю границу N для поиска простых чисел: ";
    if (!(std::cin >> n) || n < 2) {
        std::cout << "Простых чисел нет." << std::endl;
        return 0;
    }

    // Создаём множество на n + 1 элемент (от 0 до n)
    Set primes(n + 1);

    // Заполняем множество всеми числами от 2 до n
    for (size_t i = 2; i <= n; i++) {
        primes.insert(i);
    }

    // Алгоритм Решето Эратосфена
    for (size_t p = 2; p * p <= n; p++) {
        // Если число p еще не вычеркнуто, значит оно простое
        if (primes.is_member(p)) {
            // Вычеркиваем все числа, кратные p, начиная с p * p
            for (size_t k = p * p; k <= n; k += p) {
                primes.erase(k);
            }
        }
    }

    std::cout << "Простые числа до " << n << ":" << std::endl;
    std::cout << primes << std::endl;

    return 0;
}
