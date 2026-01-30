#ifndef PIECES_H
#define PIECES_H

#include "move.h"
#include <memory>
#include <vector>
#include <string>

enum class Color { White, Black };
enum class PieceType { Pawn, Rook, Knight, Bishop, Queen, King };

inline Color oppositeColor(Color c) {
    return c == Color::White ? Color::Black : Color::White;
}

class Board; // forward declaration

// Базовый класс фигуры
class Piece {
public:
    Color color;
    PieceType type;
    bool moved_ = false;

    Piece(Color c, PieceType t) : color(c), type(t) {}
    virtual ~Piece() = default;

    virtual std::string getSymbol() const = 0;
    virtual std::vector<Move> generatePseudoLegalMoves(const Square& pos, const Board& board) const = 0;
    virtual std::unique_ptr<Piece> clone() const = 0;

    // Символ для FEN-подобного представления
    char fenChar() const;
};

// Вспомогательная функция для скользящих фигур (ладья, слон, ферзь)
std::vector<Move> generateSlidingMoves(const Square& pos, const Board& board,
                                        Color color,
                                        const std::vector<std::pair<int,int>>& directions);

class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c, PieceType::Pawn) {}
    std::string getSymbol() const override;
    std::vector<Move> generatePseudoLegalMoves(const Square& pos, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override;
};

class Rook : public Piece {
public:
    Rook(Color c) : Piece(c, PieceType::Rook) {}
    std::string getSymbol() const override;
    std::vector<Move> generatePseudoLegalMoves(const Square& pos, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override;
};

class Knight : public Piece {
public:
    Knight(Color c) : Piece(c, PieceType::Knight) {}
    std::string getSymbol() const override;
    std::vector<Move> generatePseudoLegalMoves(const Square& pos, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override;
};

class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c, PieceType::Bishop) {}
    std::string getSymbol() const override;
    std::vector<Move> generatePseudoLegalMoves(const Square& pos, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override;
};

class Queen : public Piece {
public:
    Queen(Color c) : Piece(c, PieceType::Queen) {}
    std::string getSymbol() const override;
    std::vector<Move> generatePseudoLegalMoves(const Square& pos, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override;
};

class King : public Piece {
public:
    King(Color c) : Piece(c, PieceType::King) {}
    std::string getSymbol() const override;
    std::vector<Move> generatePseudoLegalMoves(const Square& pos, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override;
};

#endif
