#include <iostream>

int priceFor(int sessions, bool discounted) {
    // TODO: проверка 1..12, скидка и именованные constexpr.
    constexpr int PRICE_PER_SESSION = 150;
    constexpr int MIN_SESSIONS = 1;
    constexpr int MAX_SESSIONS = 12;
    constexpr int DISCOUNT_PERCENT = 20;
    constexpr int PERCENT_BASE = 100;
    
    if (sessions < MIN_SESSIONS || sessions > MAX_SESSIONS) return -1;
    
    int price = sessions * PRICE_PER_SESSION;
    if (discounted) {
        price = price * (PERCENT_BASE - DISCOUNT_PERCENT) / PERCENT_BASE;
    }
    return price;
}

bool finishSale(int sessions, bool discounted, int* wallet, int& receipt) {
    // TODO: разделите расчёт, проверку и запись результата.
    if (wallet == nullptr) return false;
    if (*wallet < 0) return false;
    
    int price = priceFor(sessions, discounted);
    if (price == -1) return false;          // невалидные сессии
    if (*wallet < price) return false;      // недостаточно средств
    
    *wallet -= price;
    receipt = price;
    return true;
}

int main() {
    int sessions{}, student{}, money{}, absent{};
    if (!(std::cin >> sessions >> student >> money >> absent)) return 1;
    int receipt = -99;
    bool ok = finishSale(sessions, student != 0, absent ? nullptr : &money, receipt);
    std::cout << ok << " " << money << " " << receipt << " "
              << priceFor(sessions, student != 0) << "\n";
}