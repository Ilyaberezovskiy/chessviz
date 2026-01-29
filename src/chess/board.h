#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "piece.h"
#include <array>
#include <vector>

namespace chess {

class Board {
public:
    Board();

    const Piece& getPiece(Position pos) const;
    void setPiece(Position pos, Piece piece);
    void clearSquare(Position pos);
    bool isSquareOccupied(Position pos) const;
    bool isValidPosition(Position pos) const;

    void reset();

private:
    std::array<std::array<Piece, 8>, 8> squares;
};

} // namespace chess

#endif // CHESS_BOARD_H
