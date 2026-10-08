#include <iostream>
#include "modpow.h"
#include "euclid.h"
#include "shamir.h"

int main() {
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n МЕНЮ\n";
        std::cout << "1. Задание 1: a^x mod p (Ферма и двоичное разложение)\n";
        std::cout << "2. Задание 2: c*d mod m = 1 (расширенный Евклид)\n";
        std::cout << "3. Задание 3: обратный элемент c^-1 mod m\n";
        std::cout << "4. Задание 4: протокол Шамира\n";
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
