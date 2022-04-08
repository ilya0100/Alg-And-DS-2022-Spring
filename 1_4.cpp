/*  1.4 Инвертируйте значение бита в числе N по его номеру K.
    Формат входных данных. Число N, номер бита K.
    Формат выходных данных. Число с инвертированным битом в десятичном виде. */

#include <iostream>
#include <cassert>

uint invert_bit(uint n, int k) {
    return n ^ 1 << k;
}


int main() {
    long int n = 0;
    int k = 0;

    std::cin >> n >> k;
    assert(n >= 0 && k >= 0);

    std::cout << invert_bit(n, k) << std::endl;  
}
