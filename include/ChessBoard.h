#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "Piece.h"
#include "Types.h"
#include <array>
#include <vector>

class ChessBoard {
private:
    std::array<std::array<Piece, 8>, 8> board;
    Position enPassantTarget;
    
public:
    ChessBoard();
    
    void reset();
    
    const Piece& getPiece(const Position& pos) const;
    Piece& getPieceRef(const Position& pos);
    void setPiece(const Position& pos, const Piece& piece);
    void removePiece(const Position& pos);
    
    bool isOccupied(const Position& pos) const;
    bool isOccupiedByColor(const Position& pos, Color color) const;
    
    Position getEnPassantTarget() const { return enPassantTarget; }
    void setEnPassantTarget(const Position& pos) { enPassantTarget = pos; }
    
    Position findKing(Color color) const;
    
    std::vector<Position> getAllPiecesOfColor(Color color) const;
};

#endif
