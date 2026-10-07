#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include "shamir.h"
#include "euclid.h"
#include "modpow.h"
#include "utils.h"

//открытый модуль: простое число больше 255 (для шифрования байтов)
static const int SHAMIR_P = 257;

//один проход: байт -> m^key mod p -> байт
bool processFile(std::string inName, std::string outName, int key, int p) {
    std::ifstream in(inName.c_str(), std::ios::binary);
    if (!in.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << inName << "\n";
        return false;
    }
    std::ofstream out(outName.c_str(), std::ios::binary);
    if (!out.is_open()) {
        std::cout << "Ошибка: не удалось создать файл " << outName << "\n";
        in.close();
        return false;
    }

    char ch;
    int count = 0;
    while (in.get(ch)) {
        int m = ch; //код байта
        if (m < 0) {
            m = m + 256; //приводим к диапазону 0-255
        }
        m = m + 1; //получаем число 1-256 (0 возводить нельзя)
        int c = powModQuiet(m, key, p); //c = m^key mod p
        out.put((char)(c - 1)); //записываем обратно байтом
        count = count + 1;
    }

    in.close();
    out.close();
    std::cout << "  Обработано байт: " << count << "\n";
    return true;
}

//вычисление обратного ключа key^-1 mod (p-1)
int computeInverse(int key, int p) {
    std::cout << "Поиск обратного ключа для " << key
    << " по модулю " << (p - 1) << ":\n";
    int u, v;
    int g = extendedEuclid(key, p - 1, u, v);
    if (g != 1) {
        std::cout << "НОД != 1, обратного ключа нет.\n";
        return -1;
    }
    int inv = u % (p - 1);
    if (inv < 0) {
        inv = inv + (p - 1);
    }
    std::cout << "Обратный ключ: " << inv << "\n";
    return inv;
}

//сохранение ключей в файл
bool saveKeys(std::string name, int p, int a, int b) {
    std::ofstream f(name.c_str());
    if (!f.is_open()) {
        return false;
    }
    f << p << "\n" << a << "\n" << b << "\n";
    f.close();
    return true;
}

//загрузка ключей из файла
bool loadKeys(std::string name, int &p, int &a, int &b) {
    std::ifstream f(name.c_str());
    if (!f.is_open()) {
        return false;
    }
    f >> p >> a >> b;
    f.close();
    return true;
}

//запрос ключей a и b у пользователя с проверкой
bool askKeys(int &a, int &b, int p) {
    std::cout << "Введите ключ A (взаимно простой с " << (p - 1) << "): ";
    std::cin >> a;
    if (a <= 0 || gcdSimple(a, p - 1) != 1) {
        std::cout << "Ключ A должен быть положительным и взаимно простым с "
        << (p - 1) << "\n";
        return false;
    }
    std::cout << "Введите ключ B (взаимно простой с " << (p - 1) << "): ";
    std::cin >> b;
    if (b <= 0 || gcdSimple(b, p - 1) != 1) {
        std::cout << "Ключ B должен быть положительным и взаимно простым с "
        << (p - 1) << "\n";
        return false;
    }
    return true;
}

//зашифровать файл (проходы 1 и 2)
void encrypt() {
    std::cout << "\n Шифрование файла (проходы 1 и 2)\n";
    std::cout << "Открытый модуль p = " << SHAMIR_P << "\n";

    int a, b;
    if (!askKeys(a, b, SHAMIR_P)) {
        return;
    }

    //считаем обратные ключи
    int aInv = computeInverse(a, SHAMIR_P);
    int bInv = computeInverse(b, SHAMIR_P);
    if (aInv < 0 || bInv < 0) {
        return;
    }

    std::string inName, outName;
    std::cout << "\nВведите имя исходного файла: ";
    std::cin >> inName;
    std::cout << "Введите имя зашифрованного файла: ";
    std::cin >> outName;

    std::cout << "\nПроход 1: A шифрует ключом a = " << a << "\n";
    if (!processFile(inName, "shamir_t1.bin", a, SHAMIR_P)) {
        return;
    }
    std::cout << "Проход 2: B шифрует ключом b = " << b << "\n";
    if (!processFile("shamir_t1.bin", outName, b, SHAMIR_P)) {
        return;
    }

    std::remove("shamir_t1.bin");

    //сохраняем ключи рядом с зашифрованным файлом
    std::string keyFile = outName + ".key";
    if (saveKeys(keyFile, SHAMIR_P, a, b)) {
        std::cout << "Ключи сохранены в файл: " << keyFile << "\n";
    } else {
        std::cout << "ВНИМАНИЕ: не удалось сохранить ключи, запомните их!\n";
    }

    std::cout << "Готово. Зашифрованный файл: " << outName << "\n";
}

//расшифровывыем файл (проходы 3 и 4)
void decrypt() {
    std::cout << "\n Расшифровка файла (проходы 3 и 4)\n";

    std::string inName;
    std::cout << "Введите имя зашифрованного файла: ";
    std::cin >> inName;

    int p = SHAMIR_P;
    int a, b;

    //пытаемся прочитать ключи из файла <inName>.key
    std::string keyFile = inName + ".key";
    if (!loadKeys(keyFile, p, a, b)) {
        std::cout << "Не удалось прочитать файл ключей " << keyFile << "\n";
        std::cout << "Введите ключи вручную:\n";
        p = SHAMIR_P;
        if (!askKeys(a, b, p)) {
            return;
        }
    } else {
        std::cout << "Загружены ключи из " << keyFile
        << ": p = " << p
        << ", a = " << a
        << ", b = " << b << "\n";
    }

    //вычисляем обратные ключи
    int aInv = computeInverse(a, p);
    int bInv = computeInverse(b, p);
    if (aInv < 0 || bInv < 0) {
        return;
    }

    std::string outName;
    std::cout << "\nВведите имя файла для расшифровки: ";
    std::cin >> outName;

    std::cout << "\nПроход 3: A снимает шифр ключом a' = " << aInv << "\n";
    if (!processFile(inName, "shamir_t3.bin", aInv, p)) {
        return;
    }
    std::cout << "Проход 4: B снимает шифр ключом b' = " << bInv << "\n";
    if (!processFile("shamir_t3.bin", outName, bInv, p)) {
        return;
    }

    std::remove("shamir_t3.bin");

    std::cout << "Готово. Расшифрованный файл: " << outName << "\n";
}

//точка входа задания 4 с подменю
void runShamir() {
    std::cout << "\n ЗАДАНИЕ 4: протокол Шамира \n";
    std::cout << "Схема трёх проходов:\n";
    std::cout << "  Шаг 1: A шифрует файл ключом a\n";
    std::cout << "  Шаг 2: B шифрует результат ключом b\n";
    std::cout << "  Шаг 3: A снимает свой ключ обратным a'\n";
    std::cout << "  Шаг 4: B снимает свой ключ обратным b'\n";

    std::cout << "\nВыберите действие:\n";
    std::cout << "1. Зашифровать файл\n";
    std::cout << "2. Расшифровать файл\n";
    std::cout << "Ваш выбор: ";

    int sub;
    std::cin >> sub;

    switch (sub) {
        case 1:
            encrypt();
            break;
        case 2:
            decrypt();
            break;
        default:
            std::cout << "Неверный выбор.\n";
            break;
    }
}
