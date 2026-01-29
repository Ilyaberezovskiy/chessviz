#include "input_handler.h"

namespace ui {

InputHandler::InputHandler() : selectedPos({-1, -1}) {}

void InputHandler::handleInput(sf::RenderWindow& window, sf::Event& event, chess::Game& game) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            int x = event.mouseButton.x;
            int y = event.mouseButton.y;

            if (x >= 0 && x < 800 && y >= 0 && y < 800) {
                int col = x / (800 / 8);
                int row = y / (800 / 8);
                chess::Position clickedPos = {row, col};

                if (selectedPos.row == -1) {
                    // Selecting a piece
                    const auto& piece = game.getBoard().getPiece(clickedPos);
                    if (!piece.isEmpty() && piece.getColor() == game.getCurrentPlayer()) {
                        selectedPos = clickedPos;
                        validMoves = chess::Rules::getValidMoves(game.getBoard(), selectedPos, game.getHistory());
                    }
                } else {
                    // Attempting to move
                    if (game.makeMove(selectedPos, clickedPos)) {
                        selectedPos = {-1, -1};
                        validMoves.clear();
                    } else {
                        // Reselect or deselect
                        const auto& piece = game.getBoard().getPiece(clickedPos);
                        if (!piece.isEmpty() && piece.getColor() == game.getCurrentPlayer()) {
                            selectedPos = clickedPos;
                            validMoves = chess::Rules::getValidMoves(game.getBoard(), selectedPos, game.getHistory());
                        } else {
                            selectedPos = {-1, -1};
                            validMoves.clear();
                        }
                    }
                }
            }
        }
    } else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::U) {
            game.undoMove();
            selectedPos = {-1, -1};
            validMoves.clear();
        } else if (event.key.code == sf::Keyboard::R) {
            game.reset();
            selectedPos = {-1, -1};
            validMoves.clear();
        }
    }
}

} // namespace ui
