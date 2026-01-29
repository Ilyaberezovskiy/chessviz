#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <vector>

namespace chess {

enum class PieceType {
    None,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

enum class Color {
    None,
    White,
    Black
};

enum class GameStatus {
    Normal,
    Check,
    Checkmate,
    Stalemate,
    Draw
};

struct Position {
    int row;
    int col;

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

class Piece {
public:
    Piece(PieceType type = PieceType::None, Color color = Color::None, Position pos = {-1, -1})
        : type(type), color(color), pos(pos) {}

    PieceType getType() const { return type; }
    Color getColor() const { return color; }
    Position getPosition() const { return pos; }
    void setPosition(Position newPos) { pos = newPos; }

    bool isEmpty() const { return type == PieceType::None; }

private:
    PieceType type;
    Color color;
    Position pos;
};

} // namespace chess

#endif // CHESS_PIECE_H
