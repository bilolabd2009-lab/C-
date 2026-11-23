#include <iostream>

// Этап 2. Именование и типизация:
// магические числа заменены именованными константами, коды действий и
// коды возврата стали перечислениями, у переменных понятные имена.

constexpr int kMinMoisturePercent = 0;
constexpr int kMaxMoisturePercent = 100;
constexpr int kOpenMoistureLimitPercent = 60;   // при такой влажности и выше клапан не открываем
constexpr int kPulseMoistureLimitPercent = 85;  // при такой влажности и выше импульс запрещён
constexpr int kPulseMoistureGainPercent = 20;
constexpr int kPulseWaterLiters = 5;
constexpr int kMaxOperations = 8;

// Фиксированный базовый тип int: приведение любого введённого числа определено,
// неизвестные коды просто не совпадут ни с одним элементом.
enum class Action : int {
    Exit = 0,
    Status = 1,
    OpenValve = 2,
    CloseValve = 3,
    WaterPulse = 4,
};

enum ExitCode : int {
    kExitOk = 0,
    kExitInputError = 1,
    kExitInvalidMoisture = 2,
};

void printStatus(bool isValveOpen, int moisturePercent, int waterUsedLiters) {
    std::cout << std::boolalpha;
    std::cout << "Valve open: " << isValveOpen << "\n";
    std::cout << "Moisture: " << moisturePercent << "%\n";
    std::cout << "Water used: " << waterUsedLiters << " L\n";
}

int main() {
    int moisturePercent;
    bool isValveOpen = false;
    int operationCount = 0;
    int waterUsedLiters = 0;

    std::cout << "Moisture: ";
    if (!(std::cin >> moisturePercent)) {
        std::cout << "Input error\n";
        return kExitInputError;
    }

    if (moisturePercent < kMinMoisturePercent || moisturePercent > kMaxMoisturePercent) {
        std::cout << "Invalid moisture\n";
        return kExitInvalidMoisture;
    }

    while (operationCount < kMaxOperations) {
        int actionCode;
        std::cout << "Action 0/1/2/3/4: ";

        if (!(std::cin >> actionCode)) {
            std::cout << "Input error\n";
            return kExitInputError;
        }

        const auto action = static_cast<Action>(actionCode);

        if (action == Action::Exit) {
            break;
        }
        else if (action == Action::Status) {
            operationCount++;
            std::cout << "Status\n";
            printStatus(isValveOpen, moisturePercent, waterUsedLiters);
        }
        else if (action == Action::OpenValve) {
            if (isValveOpen || moisturePercent >= kOpenMoistureLimitPercent) {
                std::cout << "Open denied\n";
            } else {
                isValveOpen = true;
                operationCount++;
                std::cout << "Valve opened\n";
                printStatus(isValveOpen, moisturePercent, waterUsedLiters);
            }
        }
        else if (action == Action::CloseValve) {
            isValveOpen = false;
            operationCount++;
            std::cout << "Valve closed\n";
            printStatus(isValveOpen, moisturePercent, waterUsedLiters);
        }
        else if (action == Action::WaterPulse) {
            if (isValveOpen || moisturePercent >= kPulseMoistureLimitPercent) {
                std::cout << "Pulse denied\n";
            } else {
                moisturePercent = moisturePercent + kPulseMoistureGainPercent;
                if (moisturePercent > kMaxMoisturePercent) {
                    moisturePercent = kMaxMoisturePercent;
                }
                waterUsedLiters = waterUsedLiters + kPulseWaterLiters;
                operationCount++;
                std::cout << "Pulse completed\n";
                printStatus(isValveOpen, moisturePercent, waterUsedLiters);
            }
        }
        else {
            std::cout << "Unknown action\n";
        }
    }

    std::cout << "Operations: " << operationCount << "\n";
    std::cout << "Total water: " << waterUsedLiters << " L\n";

    return kExitOk;
}
