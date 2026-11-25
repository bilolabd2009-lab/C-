#include <iostream>

void addCredits(int& credits, int amount) {
    // TODO: добавьте проверки перед изменением исходной переменной.
    if (credits < 0 || credits > 1000) return;
    if (amount < 1 || amount > 100) return;
    if (credits + amount > 1000) return;
    
    credits += amount;
}

int main() {
    int credits{}, amount{};
    if (!(std::cin >> credits >> amount)) return 1;
    addCredits(credits, amount);
    std::cout << credits << "\n";
}