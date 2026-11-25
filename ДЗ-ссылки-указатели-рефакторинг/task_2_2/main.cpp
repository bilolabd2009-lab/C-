#include <iostream>

void orderBounds(int& lower, int& upper) {
    if (lower > upper) {
        int temp = lower;
        lower = upper;
        upper = temp;
    }
}

int main() {
    int a{}, b{};
    if (!(std::cin >> a >> b)) return 1;
    orderBounds(a, b);
    std::cout << a << " " << b << "\n";
}