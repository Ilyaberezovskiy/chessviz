#ifndef MOVEVALIDATOR_H
#define MOVEVALIDATOR_H

#include "ChessBoard.h"
#include "Types.h"
#include <vector>

class MoveValidator {
public:
    static bool isValidMove(const ChessBoard& board, const Move& move, Color currentPlayer);
    
    static bool isSquareUnderAttack(const ChessBoard& board, const Position& pos, Color attackingColor);
    
    static bool isKingInCheck(const ChessBoard& board, Color kingColor);
    
    static bool wouldMoveCauseCheck(ChessBoard board, const Move& move, Color currentPlayer);
    
    static std::vector<Position> getLegalMoves(const ChessBoard& board, const Position& pos, Color currentPlayer);
    
    static bool canCastle(const ChessBoard& board, const Position& kingPos, 
                         const Position& rookPos, Color currentPlayer);
    
    static bool hasLegalMoves(const ChessBoard& board, Color color);
};

#endif
