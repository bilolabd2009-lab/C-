#include <iostream>

int rentalPrice(int hours, bool student) {
    constexpr int PRICE_PER_HOUR = 120;
    constexpr int MAX_HOURS = 24;
    constexpr int DISCOUNT_PERCENT = 25;
    constexpr int PERCENT_BASE = 100;
    
    // TODO: исправьте границы и замените магические числа.
    if (hours < 1 || hours > MAX_HOURS) return -1;
    
    if (student) return hours * PRICE_PER_HOUR * (PERCENT_BASE - DISCOUNT_PERCENT) / PERCENT_BASE;
    
    return hours * PRICE_PER_HOUR;
}

int main() {
    int hours{}, student{};
    if (!(std::cin >> hours >> student)) return 1;
    std::cout << rentalPrice(hours, student != 0) << "\n";
}