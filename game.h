#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"

class Game {
public:
    Game();
    Game(bool aiEnabled, Color aiColor);
    void run();

private:
    Board board_;
    Player whitePlayer_;
    Player blackPlayer_;
    Color currentTurn_;
    bool aiEnabled_;
    Color aiColor_;
    bool flipped_;

    Player& getCurrentPlayer();
    void switchTurn();
    bool isAiTurn() const;
};

#endif
