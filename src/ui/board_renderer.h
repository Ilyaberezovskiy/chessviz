#ifndef CHESS_UI_BOARD_RENDERER_H
#define CHESS_UI_BOARD_RENDERER_H

#include <SFML/Graphics.hpp>
#include "../chess/board.h"
#include "../chess/move.h"
#include <vector>

namespace ui {

class BoardRenderer {
public:
    BoardRenderer();
    void render(sf::RenderWindow& window, const chess::Board& board, 
                chess::Position selectedPos, const std::vector<chess::Move>& validMoves,
                const std::vector<chess::Move>& history, chess::GameStatus status, chess::Color currentPlayer);

private:
    sf::Color lightColor;
    sf::Color darkColor;
    sf::Color highlightColor;
    sf::Color moveColor;
    sf::Color lastMoveColor;
    sf::Color checkColor;
    sf::Font font;

    void drawGrid(sf::RenderWindow& window);
    void drawCoordinates(sf::RenderWindow& window);
    void drawHighlights(sf::RenderWindow& window, chess::Position selectedPos, 
                        const std::vector<chess::Move>& validMoves,
                        const std::vector<chess::Move>& history,
                        chess::GameStatus status, chess::Color currentPlayer,
                        const chess::Board& board);
};

} // namespace ui

#endif // CHESS_UI_BOARD_RENDERER_H
