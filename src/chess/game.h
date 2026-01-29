#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "board.h"
#include "rules.h"
#include <string>

namespace chess {

class Game {
public:
    Game();

    const Board& getBoard() const { return board; }
    Color getCurrentPlayer() const { return currentPlayer; }
    GameStatus getStatus() const { return status; }
    const std::vector<Move>& getHistory() const { return history; }

    bool makeMove(Position from, Position to, PieceType promotion = PieceType::Queen);
    void undoMove();
    void reset();

    std::string getMoveNotation(const Move& move) const;

private:
    Board board;
    Color currentPlayer;
    GameStatus status;
    std::vector<Move> history;
    std::vector<Board> boardHistory;

    void updateStatus();
    void switchPlayer();
};

} // namespace chess

#endif // CHESS_GAME_H
