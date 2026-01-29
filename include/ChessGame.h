#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "GameState.h"
#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include <memory>

class ChessGame {
private:
    sf::RenderWindow window;
    GameState gameState;
    Renderer renderer;
    
    Position selectedPiece;
    std::vector<Position> legalMoves;
    bool pieceSelected;
    bool promotionActive;
    Position promotionSquare;

public:
    ChessGame();
    
    void run();
    
private:
    void handleEvents();
    void handleClick(int x, int y);
    void handlePromotion(PieceType type);
    void update();
    void render();
    
    bool isNewGameButtonClicked(int x, int y) const;
    bool isUndoButtonClicked(int x, int y) const;
    bool isPromotionChoiceClicked(int x, int y, PieceType& outType) const;
};

#endif
