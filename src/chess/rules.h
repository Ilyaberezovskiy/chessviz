#ifndef CHESS_RULES_H
#define CHESS_RULES_H

#include "board.h"
#include "move.h"
#include <vector>

namespace chess {

class Rules {
public:
    static std::vector<Move> getValidMoves(const Board& board, Position pos, const std::vector<Move>& history);
    static bool isKingInCheck(const Board& board, Color color, const std::vector<Move>& history);
    static bool isCheckmate(const Board& board, Color color, const std::vector<Move>& history);
    static bool isStalemate(const Board& board, Color color, const std::vector<Move>& history);
    
    static bool canCastle(const Board& board, Color color, bool kingSide, const std::vector<Move>& history);
    static bool canEnPassant(const Board& board, Position from, Position to, const std::vector<Move>& history);

private:
    static std::vector<Move> getPseudoLegalMoves(const Board& board, Position pos, const std::vector<Move>& history);
    static bool isSquareAttacked(const Board& board, Position pos, Color attackerColor);
};

} // namespace chess

#endif // CHESS_RULES_H
