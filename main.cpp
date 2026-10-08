#include <iostream>
#include "modpow.h"
#include "euclid.h"
#include "shamir.h"

int main() {
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n МЕНЮ\n";
        std::cout << "1. Вычисление a^x mod p\n";
        std::cout << "2. Вычисление d из c*d mod m = 1\n";
        std::cout << "3. Вычисление обратного элемента из c^-1 mod m\n";
        std::cout << "4. Шифрование/дешифрование по протоколу Шамира\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите пункт: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                computePowerModulo();
                break;
            case 2:
                std::cout << "\n ЗАДАНИЕ 2: c*d mod m = 1\n";
                computeModularInverse();
                break;
            case 3:
                std::cout << "\n ЗАДАНИЕ 3: c^-1 mod m = d\n";
                computeModularInverse();
                break;
            case 4:
                runShamir();
                break;
            case 0:
                std::cout << "Выход из программы.\n";
                break;
            default:
                std::cout << "Неверный пункт меню. Попробуйте снова.\n";
                break;
        }
    }

    return 0;
}
