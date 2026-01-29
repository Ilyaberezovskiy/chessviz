#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include "piece.h"

namespace chess {

enum class MoveType {
    Normal,
    Castling,
    EnPassant,
    Promotion
};

struct Move {
    Position from;
    Position to;
    MoveType type;
    PieceType promotionType;
    Piece capturedPiece;

    Move(Position from = {-1, -1}, Position to = {-1, -1}, 
         MoveType type = MoveType::Normal, PieceType promotion = PieceType::None)
        : from(from), to(to), type(type), promotionType(promotion) {}
};

} // namespace chess

#endif // CHESS_MOVE_H
