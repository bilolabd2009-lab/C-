#include <iostream>
#include <string>

using namespace std;

void printStatus(int klapan, int vlaga, int voda) {
    cout << "Valve open: " << (klapan ? "true" : "false") << "\n";
    cout << "Moisture: " << vlaga << "%\n";
    cout << "Water used: " << voda << " L\n";
}

int main() {
    int vlaga = 0;
    int klapan = 0;
    int uspeh = 0;
    int voda = 0;

    cout << "Moisture: ";
    if (!(cin >> vlaga)) {
        cout << "Input error\n";
        return 1;
    }
    if (vlaga < 0 || vlaga > 100) {
        cout << "Invalid moisture\n";
        return 2;
    }

    while (uspeh < 8) {
        cout << "Action 0/1/2/3/4: ";
        int act;
        if (!(cin >> act)) {
            cout << "Input error\n";
            return 1;
        }

        if (act == 0) break;

        bool ok = false;
        string res = "";

        switch (act) {
            case 1:
                ok = true;
                res = "Status";
                break;

            case 2:
                if (klapan == 0 && vlaga < 60) {
                    klapan = 1;
                    ok = true;
                    res = "Valve opened";
                } else {
                    cout << "Open denied\n";
                }
                break;

            case 3:
                klapan = 0;
                ok = true;
                res = "Valve closed";
                break;

            case 4:
                if (klapan == 0 && vlaga < 85) {
                    vlaga += 20;
                    if (vlaga > 100) vlaga = 100;
                    voda += 5;
                    ok = true;
                    res = "Pulse completed";
                } else {
                    cout << "Pulse denied\n";
                }
                break;

            default:
                cout << "Unknown action\n";
                break;
        }

        if (ok) {
            uspeh++;
            cout << res << "\n";
            printStatus(klapan, vlaga, voda);
        }
    }

    cout << "Operations: " << uspeh << "\n";
    cout << "Total water: " << voda << " L\n";
    return 0;
}
