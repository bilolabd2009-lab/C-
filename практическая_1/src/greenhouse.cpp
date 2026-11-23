#include <iostream>
#include <istream>
#include <ostream>

constexpr int kMinMoisturePercent = 0;
constexpr int kMaxMoisturePercent = 100;
constexpr int kOpenMoistureLimitPercent = 60;
constexpr int kPulseMoistureLimitPercent = 85;
constexpr int kPulseMoistureGainPercent = 20;
constexpr int kPulseWaterLiters = 5;
constexpr int kMaxOperations = 8;

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

struct GreenhouseState {
    bool isValveOpen = false;
    int moisturePercent = 0;
    int waterUsedLiters = 0;
    int operationCount = 0;
};

struct Outcome {
    GreenhouseState state;
    const char* message;
    bool completed;
};

constexpr bool isValidMoisture(int moisturePercent) {
    return moisturePercent >= kMinMoisturePercent && moisturePercent <= kMaxMoisturePercent;
}

constexpr bool canOpenValve(const GreenhouseState& state) {
    return !state.isValveOpen && state.moisturePercent < kOpenMoistureLimitPercent;
}

constexpr bool canWaterPulse(const GreenhouseState& state) {
    return !state.isValveOpen && state.moisturePercent < kPulseMoistureLimitPercent;
}

constexpr int moistureAfterPulse(int moisturePercent) {
    const int raised = moisturePercent + kPulseMoistureGainPercent;
    return raised > kMaxMoisturePercent ? kMaxMoisturePercent : raised;
}

constexpr Action toAction(int actionCode) {
    return static_cast<Action>(actionCode);
}

constexpr bool hasOperationsLeft(const GreenhouseState& state) {
    return state.operationCount < kMaxOperations;
}

constexpr Outcome completed(GreenhouseState state, const char* message) {
    ++state.operationCount;
    return {state, message, true};
}

constexpr Outcome rejected(const GreenhouseState& state, const char* message) {
    return {state, message, false};
}

constexpr Outcome applyAction(const GreenhouseState& state, Action action) {
    switch (action) {
        case Action::Status:
            return completed(state, "Status");
        case Action::OpenValve: {
            if (!canOpenValve(state)) {
                return rejected(state, "Open denied");
            }
            GreenhouseState next = state;
            next.isValveOpen = true;
            return completed(next, "Valve opened");
        }
        case Action::CloseValve: {
            GreenhouseState next = state;
            next.isValveOpen = false;
            return completed(next, "Valve closed");
        }
        case Action::WaterPulse: {
            if (!canWaterPulse(state)) {
                return rejected(state, "Pulse denied");
            }
            GreenhouseState next = state;
            next.moisturePercent = moistureAfterPulse(state.moisturePercent);
            next.waterUsedLiters += kPulseWaterLiters;
            return completed(next, "Pulse completed");
        }
        default:
            return rejected(state, "Unknown action");
    }
}

static_assert(isValidMoisture(0) && isValidMoisture(100));
static_assert(!isValidMoisture(-1) && !isValidMoisture(101));
static_assert(moistureAfterPulse(30) == 50 && moistureAfterPulse(84) == 100);
static_assert(applyAction({false, 59, 0, 0}, Action::OpenValve).completed);
static_assert(!applyAction({false, 60, 0, 0}, Action::OpenValve).completed);
static_assert(!applyAction({true, 10, 0, 0}, Action::OpenValve).completed);
static_assert(!applyAction({false, 85, 0, 0}, Action::WaterPulse).completed);
static_assert(!applyAction({true, 10, 0, 0}, Action::WaterPulse).completed);
static_assert(applyAction({false, 84, 0, 0}, Action::WaterPulse).state.moisturePercent == 100);
static_assert(applyAction({false, 30, 0, 0}, Action::WaterPulse).state.waterUsedLiters == 5);
static_assert(applyAction({false, 30, 0, 0}, Action::CloseValve).state.operationCount == 1);
static_assert(applyAction({false, 30, 0, 0}, toAction(9)).state.operationCount == 0);

const char* boolText(bool value) {
    return value ? "true" : "false";
}

void printStatus(std::ostream& out, const GreenhouseState& state) {
    out << "Valve open: " << boolText(state.isValveOpen) << "\n";
    out << "Moisture: " << state.moisturePercent << "%\n";
    out << "Water used: " << state.waterUsedLiters << " L\n";
}

void printOutcome(std::ostream& out, const Outcome& outcome) {
    out << outcome.message << "\n";
    if (outcome.completed) {
        printStatus(out, outcome.state);
    }
}

void printSummary(std::ostream& out, const GreenhouseState& state) {
    out << "Operations: " << state.operationCount << "\n";
    out << "Total water: " << state.waterUsedLiters << " L\n";
}

bool promptInt(std::istream& in, std::ostream& out, const char* prompt, int& value) {
    out << prompt;
    if (in >> value) {
        return true;
    }
    out << "Input error\n";
    return false;
}

int run(std::istream& in, std::ostream& out) {
    int moisturePercent = 0;
    if (!promptInt(in, out, "Moisture: ", moisturePercent)) {
        return kExitInputError;
    }
    if (!isValidMoisture(moisturePercent)) {
        out << "Invalid moisture\n";
        return kExitInvalidMoisture;
    }

    GreenhouseState state;
    state.moisturePercent = moisturePercent;

    while (hasOperationsLeft(state)) {
        int actionCode = 0;
        if (!promptInt(in, out, "Action 0/1/2/3/4: ", actionCode)) {
            return kExitInputError;
        }
        const Action action = toAction(actionCode);
        if (action == Action::Exit) {
            break;
        }
        const Outcome outcome = applyAction(state, action);
        printOutcome(out, outcome);
        state = outcome.state;
    }

    printSummary(out, state);
    return kExitOk;
}

int main() {
    return run(std::cin, std::cout);
}
