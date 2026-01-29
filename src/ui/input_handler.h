#ifndef CHESS_UI_INPUT_HANDLER_H
#define CHESS_UI_INPUT_HANDLER_H

#include <SFML/Graphics.hpp>
#include "../chess/game.h"

namespace ui {

class InputHandler {
public:
    InputHandler();
    void handleInput(sf::RenderWindow& window, sf::Event& event, chess::Game& game);

    chess::Position getSelectedPosition() const { return selectedPos; }
    std::vector<chess::Move> getValidMoves() const { return validMoves; }

private:
    chess::Position selectedPos;
    std::vector<chess::Move> validMoves;
};

} // namespace ui

#endif // CHESS_UI_INPUT_HANDLER_H
