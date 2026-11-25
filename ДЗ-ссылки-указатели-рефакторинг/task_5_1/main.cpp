// ЗАДАНИЕ 5.1 — КОД-РЕВЬЮ
// Прочитайте программу и ответьте на вопросы в трёх полях слева.
// В примере специально оставлены проблемы для анализа.
// Переписывать код справа для сдачи не требуется.

#include <iostream>

// Передача по значению: balance — локальная копия.
void addCopy(int balance, int amount)
{
    if (amount > 0)
        balance += amount;
}

// Передача по ссылке: можно изменить исходную переменную.
void addReference(int& balance, int amount)
{
    if (amount > 0)
        balance += amount;
}

// Передача по указателю: проверьте безопасность условий.
bool payWithPointer(int* balance, int price)
{
    if (*balance >= price)
    {
        *balance -= price;
        return true;
    }
    return false;
}

int main()
{
    int balance{100};

    addCopy(balance, 25);
    addReference(balance, 25);

    const bool paid = payWithPointer(&balance, 30);

    std::cout << "balance: " << balance << "\n";
    std::cout << "paid: " << paid << "\n";

    return 0;
}

// ВОПРОСЫ ДЛЯ АНАЛИЗА
// 1. Каков вывод и почему int, int& и int* ведут себя так?
// 2. Что произойдёт при nullptr или отрицательной цене?
//    Где и в каком порядке нужны проверки?
// 3. Как улучшить структуру похожего кода в заданиях
//    1.1, 4.1 или 4.2? Приведите пример своего решения.
