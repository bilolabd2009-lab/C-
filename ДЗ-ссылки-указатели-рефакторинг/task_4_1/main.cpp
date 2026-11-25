#include <iostream>

// TODO: уточните условия и выделите единую проверку.
bool canShip(int stock, int requested) {
    return stock >= 0 && requested > 0 && requested <= stock;
}

bool ship(int& stock, int requested) {
    if (!canShip(stock, requested)) return false;
    stock -= requested;
    return true;
}

int main() {
    int stock{}, requested{};
    if (!(std::cin >> stock >> requested)) return 1;
    const bool success = ship(stock, requested);
    std::cout << success << " " << stock << " "
              << canShip(stock, requested) << "\n";
}