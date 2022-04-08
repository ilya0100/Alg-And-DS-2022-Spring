#include <iostream>
#include <bitset>
#define MAX_NUMBER (4294967295 - 1)
void check_num(uint32_t number) {
    short counter = 0;
    if (number > MAX_NUMBER ) {
        std::cout << "FAIL" << std::endl;
        return ;
    }
    while (number) {
        if (number & 1) {
            counter++;
        }
        number >>= 1;
    }
    
    if (counter == 1) {
        std::cout << "OK" << std::endl;
        return ;
    } else {
        std::cout << "FAIL" << std::endl;
        return ;
    }
}
int main() {
    uint32_t number = 0;
    std::cin >> number;
    check_num(number);
    return 0;
}