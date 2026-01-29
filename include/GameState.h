#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "ChessBoard.h"
#include "Types.h"
#include <vector>
#include <string>

struct HistoryEntry {
    Move move;
    Piece capturedPiece;
    Position enPassantTarget;
    bool kingMoved;
    bool rookMoved;
};

class GameState {
private:
    ChessBoard board;
    Color currentPlayer;
    GameStatus status;
    std::vector<HistoryEntry> history;
    Move lastMove;
    int halfMoveClock;
    int fullMoveNumber;
    
    bool whiteKingMoved;
    bool whiteRookKingsideMoved;
    bool whiteRookQueensideMoved;
    bool blackKingMoved;
    bool blackRookKingsideMoved;
    bool blackRookQueensideMoved;

public:
    GameState();
    
    void reset();
    
    bool makeMove(const Move& move);
    bool undoMove();
    
    const ChessBoard& getBoard() const { return board; }
    Color getCurrentPlayer() const { return currentPlayer; }
    GameStatus getStatus() const { return status; }
    Move getLastMove() const { return lastMove; }
    
    std::vector<Position> getLegalMovesForPiece(const Position& pos);
    
    bool canCastleKingside(Color color) const;
    bool canCastleQueenside(Color color) const;
    
private:
    void updateGameStatus();
    void switchPlayer();
    bool isThreefoldRepetition() const;
    std::string getBoardHash() const;
};

#endif
