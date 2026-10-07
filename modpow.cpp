#include <iostream>
#include "modpow.h"
#include "utils.h"

//возведение base^exp mod mod умножением в цикле
int powModQuiet(int base, int exp, int mod) {
    int result = 1;
    int b = base % mod;
    if (b < 0) {
        b = b + mod;
    }
    for (int i = 0; i < exp; i++) {
        result = (result * b) % mod;
    }
    return result;
}

//метод 2 разложение степени в двоичный вид
int modPowBinary(int a, int x, int p) {
    std::cout << "\n Метод 2: разложение степени в двоичный вид\n";

    int base = a % p;
    if (base < 0) {
        base = base + p;
    }
    std::cout << "Основание: a mod p = " << base << "\n";

    //записываем x в двоичном виде
    int bits[64];
    int n = 0;
    int temp = x;
    if (temp == 0) {
        bits[0] = 0;
        n = 1;
    }
    while (temp > 0) {
        bits[n] = temp % 2;
        temp = temp / 2;
        n = n + 1;
    }

    std::cout << "Показатель x = " << x << " в двоичном виде: ";
    for (int i = n - 1; i >= 0; i--) {
        std::cout << bits[i];
    }
    std::cout << "\n";

    //идём по битам от младшего к старшему
    int result = 1;
    for (int i = 0; i < n; i++) {
        std::cout << "Бит " << i << " = " << bits[i] << ": ";
        if (bits[i] == 1) {
            result = (result * base) % p;
            std::cout << "умножаем result на base -> result = " << result << "; ";
        } else {
            std::cout << "умножение не нужно; ";
        }
        base = (base * base) % p;
        std::cout << "base = base^2 mod p = " << base << "\n";
    }
    std::cout << "Итог: " << a << "^" << x << " mod " << p << " = " << result << "\n";
    return result;
}

//задание 1
void computePowerModulo() {
    std::cout << "\n ЗАДАНИЕ 1: вычисление a^x mod p\n";

    int a, x, p;
    std::cout << "Введите основание a: ";
    std::cin >> a;
    std::cout << "Введите степень x (целое, >= 0): ";
    std::cin >> x;
    std::cout << "Введите модуль p: ";
    std::cin >> p;

    //проверки корректности ввода
    if (p <= 1) {
        std::cout << "Модуль должен быть больше 1.\n";
        return;
    }
    if (x < 0) {
        std::cout << "Степень должна быть неотрицательной.\n";
        return;
    }

    int aMod = a % p;
    if (aMod < 0) {
        aMod = aMod + p;
    }
    std::cout << "\nПриводим основание по модулю: a mod p = " << aMod << "\n";

    //метод 1 теорема Ферма
    std::cout << "\n Метод 1: через теорему Ферма\n";
    std::cout << "Теорема Ферма: если p простое и НОД(a, p) = 1,\n";
    std::cout << "то a^(p-1) = 1 (mod p), значит показатель можно уменьшить по модулю (p-1).\n";

    bool prime = isPrime(p);
    std::cout << "Проверка простоты p = " << p << ": ";
    if (prime) {
        std::cout << "число простое\n";
    } else {
        std::cout << "число составное\n";
    }

    bool fermatOk = false;
    if (prime) {
        int g = gcdSimple(aMod, p);
        std::cout << "Проверка НОД(a, p) = " << g << ": ";
        if (g == 1) {
            std::cout << "условие теоремы выполнено\n";
            fermatOk = true;
        } else {
            std::cout << "условие теоремы не выполнено\n";
        }
    }

    if (fermatOk) {
        int e = x % (p - 1);
        std::cout << "Уменьшаем показатель: x mod (p-1) = " << x
        << " mod " << (p - 1) << " = " << e << "\n";
        int r1 = powModQuiet(aMod, e, p);
        std::cout << "Получаем a^" << e << " mod " << p << " = " << r1 << "\n";
        std::cout << "Ответ по теореме Ферма: " << r1 << "\n";
    } else {
        std::cout << "Метод 1 применить нельзя.\n";
    }

    //метод 2 двоичное разложение
    int r2 = modPowBinary(aMod, x, p);
    std::cout << "Ответ по методу двоичного разложения: " << r2 << "\n";
}
