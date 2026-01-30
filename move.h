#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <optional>

// Клетка доски (row 0 = rank 1, col 0 = file a)
struct Square {
    int row;
    int col;

    bool isValid() const { return row >= 0 && row < 8 && col >= 0 && col < 8; }
    bool operator==(const Square& other) const { return row == other.row && col == other.col; }
    bool operator!=(const Square& other) const { return !(*this == other); }
    std::string toString() const;
};

// Ход: откуда, куда, превращение (опционально)
struct Move {
    Square from;
    Square to;
    char promotion = '\0'; // 'q', 'r', 'b', 'n' или '\0'

    std::string toString() const;
};

// Парсинг хода из строки: "e2 e4", "e2e4", "e7e8q"
std::optional<Move> parseMove(const std::string& input);

#endif
