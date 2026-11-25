#include <iostream>

bool charge(int* wallet, int cost) {
    // TODO: напишите защитные проверки, потом уменьшите *wallet.
    if (wallet == nullptr) return false;
    if (cost <= 0) return false;
    if (*wallet < 0) return false;
    if (*wallet < cost) return false;
    
    *wallet -= cost;
    return true;
}

int main() {
    int money{}, cost{}, absent{};
    if (!(std::cin >> money >> cost >> absent)) return 1;
    const bool ok = charge(absent ? nullptr : &money, cost);
    std::cout << ok << " " << money << "\n";
}