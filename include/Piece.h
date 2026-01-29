#ifndef PIECE_H
#define PIECE_H

#include "Types.h"
#include <vector>

class Piece {
private:
    PieceType type;
    Color color;
    bool hasMoved;

public:
    Piece();
    Piece(PieceType t, Color c);
    
    PieceType getType() const { return type; }
    Color getColor() const { return color; }
    bool getHasMoved() const { return hasMoved; }
    void setHasMoved(bool moved) { hasMoved = moved; }
    
    bool isEmpty() const { return type == PieceType::NONE; }
    
    std::vector<Position> getPossibleMoves(const Position& pos, const class ChessBoard& board) const;
    
private:
    std::vector<Position> getPawnMoves(const Position& pos, const ChessBoard& board) const;
    std::vector<Position> getKnightMoves(const Position& pos, const ChessBoard& board) const;
    std::vector<Position> getBishopMoves(const Position& pos, const ChessBoard& board) const;
    std::vector<Position> getRookMoves(const Position& pos, const ChessBoard& board) const;
    std::vector<Position> getQueenMoves(const Position& pos, const ChessBoard& board) const;
    std::vector<Position> getKingMoves(const Position& pos, const ChessBoard& board) const;
    
    void addDirectionalMoves(std::vector<Position>& moves, const Position& pos, 
                            int rowDir, int colDir, const ChessBoard& board,
                            bool unlimited = true) const;
};

#endif
