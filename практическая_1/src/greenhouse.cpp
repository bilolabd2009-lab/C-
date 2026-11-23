#include <iostream>

// Этап 2. Именование и типизация: константы, перечисления, понятные имена.
// Этап 3. Выделение чистых функций: правила предметной области вынесены
// в функции без ввода-вывода и без побочных эффектов.
// Этап 4. Устранение дублирования вывода: печать сообщения + статуса,
// приглашение с чтением числа и сообщение об ошибке ввода — по одному разу.

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

// ---- Чистые функции: результат зависит только от аргументов ----

constexpr bool isValidMoisture(int moisturePercent) {
    return moisturePercent >= kMinMoisturePercent && moisturePercent <= kMaxMoisturePercent;
}

constexpr bool canOpenValve(bool isValveOpen, int moisturePercent) {
    return !isValveOpen && moisturePercent < kOpenMoistureLimitPercent;
}

constexpr bool canWaterPulse(bool isValveOpen, int moisturePercent) {
    return !isValveOpen && moisturePercent < kPulseMoistureLimitPercent;
}

constexpr int moistureAfterPulse(int moisturePercent) {
    const int raised = moisturePercent + kPulseMoistureGainPercent;
    return raised > kMaxMoisturePercent ? kMaxMoisturePercent : raised;
}

constexpr Action toAction(int actionCode) {
    return static_cast<Action>(actionCode);
}

// Правила проверяются ещё при компиляции.
static_assert(isValidMoisture(0) && isValidMoisture(100));
static_assert(!isValidMoisture(-1) && !isValidMoisture(101));
static_assert(canOpenValve(false, 59) && !canOpenValve(false, 60) && !canOpenValve(true, 10));
static_assert(canWaterPulse(false, 84) && !canWaterPulse(false, 85) && !canWaterPulse(true, 10));
static_assert(moistureAfterPulse(30) == 50 && moistureAfterPulse(84) == 100);

// ---- Ввод-вывод ----

void printStatus(bool isValveOpen, int moisturePercent, int waterUsedLiters) {
    std::cout << std::boolalpha;
    std::cout << "Valve open: " << isValveOpen << "\n";
    std::cout << "Moisture: " << moisturePercent << "%\n";
    std::cout << "Water used: " << waterUsedLiters << " L\n";
}

// Печатает приглашение и читает целое число. При ошибке сам сообщает о ней.
bool promptInt(const char* prompt, int& value) {
    std::cout << prompt;
    if (std::cin >> value) {
        return true;
    }
    std::cout << "Input error\n";
    return false;
}

int main() {
    int moisturePercent;
    bool isValveOpen = false;
    int operationCount = 0;
    int waterUsedLiters = 0;

    if (!promptInt("Moisture: ", moisturePercent)) {
        return kExitInputError;
    }

    if (!isValidMoisture(moisturePercent)) {
        std::cout << "Invalid moisture\n";
        return kExitInvalidMoisture;
    }

    while (operationCount < kMaxOperations) {
        int actionCode;
        if (!promptInt("Action 0/1/2/3/4: ", actionCode)) {
            return kExitInputError;
        }

        const Action action = toAction(actionCode);
        if (action == Action::Exit) {
            break;
        }

        // Ветки только решают, что произошло. Успешная операция задаёт
        // completedMessage, а счёт и печать статуса выполняются ниже один раз.
        const char* completedMessage = nullptr;
        const char* rejectedMessage = nullptr;

        switch (action) {
            case Action::Status:
                completedMessage = "Status";
                break;
            case Action::OpenValve:
                if (canOpenValve(isValveOpen, moisturePercent)) {
                    isValveOpen = true;
                    completedMessage = "Valve opened";
                } else {
                    rejectedMessage = "Open denied";
                }
                break;
            case Action::CloseValve:
                isValveOpen = false;
                completedMessage = "Valve closed";
                break;
            case Action::WaterPulse:
                if (canWaterPulse(isValveOpen, moisturePercent)) {
                    moisturePercent = moistureAfterPulse(moisturePercent);
                    waterUsedLiters += kPulseWaterLiters;
                    completedMessage = "Pulse completed";
                } else {
                    rejectedMessage = "Pulse denied";
                }
                break;
            default:
                rejectedMessage = "Unknown action";
                break;
        }

        if (completedMessage != nullptr) {
            operationCount++;
            std::cout << completedMessage << "\n";
            printStatus(isValveOpen, moisturePercent, waterUsedLiters);
        } else {
            std::cout << rejectedMessage << "\n";
        }
    }

    std::cout << "Operations: " << operationCount << "\n";
    std::cout << "Total water: " << waterUsedLiters << " L\n";

    return kExitOk;
}
