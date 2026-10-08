#include <iostream>
#include "euclid.h"
#include "utils.h"

//расширенный алгоритм Евклида с печатью всех шагов
int extendedEuclid(int a, int b, int &u, int &v) {
    int old_r = a;
    int r = b;
    int old_u = 1;
    int cur_u = 0;
    int old_v = 0;
    int cur_v = 1;
    int step = 0;

    std::cout << "Начало: r = " << old_r << ", u = " << old_u << ", v = " << old_v << "\n";

    while (r != 0) {
        int q = old_r / r;
        std::cout << "Шаг " << step << ": " << old_r << " = " << q << " * " << r
        << " + " << (old_r - q * r) << "\n";

        int t = old_r - q * r;
        old_r = r;
        r = t;

        t = old_u - q * cur_u;
        old_u = cur_u;
        cur_u = t;

        t = old_v - q * cur_v;
        old_v = cur_v;
        cur_v = t;

        std::cout << "        r = " << old_r << ", u = " << old_u
        << ", v = " << old_v << "\n";
        step = step + 1;
    }

    u = old_u;
    v = old_v;

    std::cout << "НОД(" << a << ", " << b << ") = " << old_r << "\n";
    std::cout << "Коэффициенты: u = " << u << ", v = " << v << "\n";
    std::cout << "Проверка: " << a << "*" << u << " + " << b << "*" << v
    << " = " << (a * u + b * v) << "\n";

    return old_r;
}

//задание 2 и 3
void computeModularInverse() {
    int c, m;
    std::cout << "Введите число c: ";
    std::cin >> c;
    std::cout << "Введите модуль m: ";
    std::cin >> m;

    if (m <= 1) {
        std::cout << "Модуль должен быть больше 1.\n";
        return;
    }
    if (c <= 0) {
        std::cout << "Число c должно быть положительным.\n";
        return;
    }

    int u, v;
    int g = extendedEuclid(c, m, u, v);

    if (g != 1) {
        std::cout << "НОД(c, m) = " << g << " != 1, обратного числа нет.\n";
        return;
    }

    int d = u % m;
    if (d < 0) {
        d = d + m;
    }

    std::cout << "\nОбратное число: c^-1 mod m = d = " << d << "\n";
    int check = ((c % m) * (d % m)) % m;
    std::cout << "Проверка: " << c << " * " << d << " mod " << m
    << " = " << check << "\n";
}
