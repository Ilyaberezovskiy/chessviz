#ifndef CHESS_UI_PIECE_RENDERER_H
#define CHESS_UI_PIECE_RENDERER_H

#include <SFML/Graphics.hpp>
#include "../chess/board.h"

namespace ui {

class PieceRenderer {
public:
    PieceRenderer() = default;
    void render(sf::RenderWindow& window, const chess::Board& board);

private:
    // Функции рисования фигур
    void drawPawn(sf::RenderTarget& target, float x, float y, bool isWhite);
    void drawKnight(sf::RenderTarget& target, float x, float y, bool isWhite);
    void drawBishop(sf::RenderTarget& target, float x, float y, bool isWhite);
    void drawRook(sf::RenderTarget& target, float x, float y, bool isWhite);
    void drawQueen(sf::RenderTarget& target, float x, float y, bool isWhite);
    void drawKing(sf::RenderTarget& target, float x, float y, bool isWhite);
    
    sf::Color getColorForPiece(bool isWhite);
};

} // namespace ui

#endif // CHESS_UI_PIECE_RENDERER_H
