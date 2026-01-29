#ifndef CHESS_UI_PIECE_RENDERER_H
#define CHESS_UI_PIECE_RENDERER_H

#include <SFML/Graphics.hpp>
#include "../chess/board.h"
#include <map>
#include <string>

namespace ui {

class PieceRenderer {
public:
    PieceRenderer();
    void render(sf::RenderWindow& window, const chess::Board& board);

private:
    std::map<std::string, sf::Texture> textures;
    void loadTextures();
};

} // namespace ui

#endif // CHESS_UI_PIECE_RENDERER_H
