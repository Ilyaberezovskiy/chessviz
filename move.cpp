#include "move.h"
#include <algorithm>
#include <cctype>

std::string Square::toString() const {
    std::string s;
    s += static_cast<char>('a' + col);
    s += static_cast<char>('1' + row);
    return s;
}

std::string Move::toString() const {
    std::string s = from.toString() + to.toString();
    if (promotion != '\0') {
        s += promotion;
    }
    return s;
}

// Парсинг хода из строки
// Поддерживаемые форматы: "e2 e4", "e2e4", "e7e8q"
std::optional<Move> parseMove(const std::string& input) {
    std::string s;
    for (char c : input) {
        if (c != ' ') {
            s += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
    }

    // Минимум 4 символа: e2e4
    if (s.size() < 4 || s.size() > 5) {
        return std::nullopt;
    }

    // Парсинг клетки "откуда"
    if (s[0] < 'a' || s[0] > 'h' || s[1] < '1' || s[1] > '8') {
        return std::nullopt;
    }
    // Парсинг клетки "куда"
    if (s[2] < 'a' || s[2] > 'h' || s[3] < '1' || s[3] > '8') {
        return std::nullopt;
    }

    Move move;
    move.from = {s[1] - '1', s[0] - 'a'};
    move.to = {s[3] - '1', s[2] - 'a'};

    // Превращение пешки
    if (s.size() == 5) {
        char p = s[4];
        if (p == 'q' || p == 'r' || p == 'b' || p == 'n') {
            move.promotion = p;
        } else {
            return std::nullopt;
        }
    }

    return move;
}
