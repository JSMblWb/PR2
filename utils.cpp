#include "utils.h"

//проверка простоты перебором делителей
bool isPrime(int n) {
    if (n < 2) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }
    for (int i = 3; i * i <= n; i = i + 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

//нод по алгоритму Евклида
int gcdSimple(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}
