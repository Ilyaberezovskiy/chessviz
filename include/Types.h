#ifndef TYPES_H
#define TYPES_H

#include <utility>

enum class PieceType {
    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

enum class Color {
    NONE,
    WHITE,
    BLACK
};

struct Position {
    int row;
    int col;
    
    Position() : row(-1), col(-1) {}
    Position(int r, int c) : row(r), col(c) {}
    
    bool isValid() const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }
    
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
    
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

struct Move {
    Position from;
    Position to;
    bool isCapture;
    bool isCastling;
    bool isEnPassant;
    bool isPromotion;
    PieceType promotionType;
    
    Move() : isCapture(false), isCastling(false), isEnPassant(false), 
             isPromotion(false), promotionType(PieceType::NONE) {}
    Move(Position f, Position t) : from(f), to(t), isCapture(false), 
                                   isCastling(false), isEnPassant(false),
                                   isPromotion(false), promotionType(PieceType::NONE) {}
};

enum class GameStatus {
    PLAYING,
    CHECK,
    CHECKMATE,
    STALEMATE,
    DRAW
};

#endif
