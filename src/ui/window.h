#ifndef CHESS_UI_WINDOW_H
#define CHESS_UI_WINDOW_H

#include <SFML/Graphics.hpp>
#include "../chess/game.h"
#include "board_renderer.h"
#include "piece_renderer.h"
#include "input_handler.h"

namespace ui {

class Window {
public:
    Window();
    void run();

private:
    sf::RenderWindow window;
    chess::Game game;
    BoardRenderer boardRenderer;
    PieceRenderer pieceRenderer;
    InputHandler inputHandler;
    sf::Font font;

    void processEvents();
    void update();
    void render();
    void drawPanel();
};

} // namespace ui

#endif // CHESS_UI_WINDOW_H
