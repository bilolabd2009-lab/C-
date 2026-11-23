#include <iostream>

void printStatus(bool valve, int moisture, int water) {
    std::cout << std::boolalpha;
    std::cout << "Valve open: " << valve << "\n";
    std::cout << "Moisture: " << moisture << "%\n";
    std::cout << "Water used: " << water << " L\n";
}

int main() {
    int moisture;
    bool valve = false;
    int operations = 0;
    int waterUsed = 0;

    std::cout << "Moisture: ";
    if (!(std::cin >> moisture)) {
        std::cout << "Input error\n";
        return 1;
    }
    
    if (moisture < 0 || moisture > 100) {
        std::cout << "Invalid moisture\n";
        return 2;
    }

    while (operations < 8) {
        int code;
        std::cout << "Action 0/1/2/3/4: ";
        
        if (!(std::cin >> code)) {
            std::cout << "Input error\n";
            return 1;
        }

        if (code == 0) {
            break;
        }
        else if (code == 1) {
            operations++;
            std::cout << "Status\n";
            printStatus(valve, moisture, waterUsed);
        }
        else if (code == 2) {
            if (valve == true || moisture >= 60) {
                std::cout << "Open denied\n";
            } else {
                valve = true;
                operations++;
                std::cout << "Valve opened\n";
                printStatus(valve, moisture, waterUsed);
            }
        }
        else if (code == 3) {
            valve = false;
            operations++;
            std::cout << "Valve closed\n";
            printStatus(valve, moisture, waterUsed);
        }
        else if (code == 4) {
            if (valve == true || moisture >= 85) {
                std::cout << "Pulse denied\n";
            } else {
                moisture = moisture + 20;
                if (moisture > 100) {
                    moisture = 100;
                }
                waterUsed = waterUsed + 5;
                operations++;
                std::cout << "Pulse completed\n";
                printStatus(valve, moisture, waterUsed);
            }
        }
        else {
            std::cout << "Unknown action\n";
        }
    }

    std::cout << "Operations: " << operations << "\n";
    std::cout << "Total water: " << waterUsed << " L\n";

    return 0;
}