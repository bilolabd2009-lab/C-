#include <iostream>

enum class ServiceState : int { New = 1, Approved = 2, Closed = 3 };

int stateCode(ServiceState state) {
    switch (state) {
        case ServiceState::New:      return 10;
        case ServiceState::Approved: return 20;
        case ServiceState::Closed:   return 30;
        default:                     return -1;
    }
}

int main() {
    int value{};
    if (!(std::cin >> value)) return 1;
    std::cout << stateCode(static_cast<ServiceState>(value)) << "\n";
}