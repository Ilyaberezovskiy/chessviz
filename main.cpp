#include "game.h"
#include <locale>
#include <iostream>
#include <string>

int main() {
    // Установка локали для корректного отображения Unicode-символов
    std::locale::global(std::locale(""));
    std::cout.imbue(std::locale());

    std::cout << "=== Выберите режим игры ===\n";
    std::cout << "1. Игрок vs Игрок\n";
    std::cout << "2. Игрок vs Компьютер\n";
    std::cout << "Выбор: ";

    std::string choice;
    if (!std::getline(std::cin, choice)) return 0;

    if (choice == "2") {
        std::cout << "\nВыберите цвет:\n";
        std::cout << "1. Белые\n";
        std::cout << "2. Чёрные\n";
        std::cout << "Выбор: ";

        std::string colorChoice;
        if (!std::getline(std::cin, colorChoice)) return 0;

        Color aiColor = (colorChoice == "2") ? Color::White : Color::Black;
        Game game(true, aiColor);
        game.run();
    } else {
        Game game;
        game.run();
    }

    return 0;
}
