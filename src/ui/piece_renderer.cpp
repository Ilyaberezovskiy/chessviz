#include "piece_renderer.h"

namespace ui {

PieceRenderer::PieceRenderer() {
    loadTextures();
}

void PieceRenderer::loadTextures() {
    std::vector<std::pair<std::string, std::string>> files = {
        {"wP", "assets/pieces/wP.png"}, {"wR", "assets/pieces/wR.png"},
        {"wN", "assets/pieces/wN.png"}, {"wB", "assets/pieces/wB.png"},
        {"wQ", "assets/pieces/wQ.png"}, {"wK", "assets/pieces/wK.png"},
        {"bP", "assets/pieces/bP.png"}, {"bR", "assets/pieces/bR.png"},
        {"bN", "assets/pieces/bN.png"}, {"bB", "assets/pieces/bB.png"},
        {"bQ", "assets/pieces/bQ.png"}, {"bK", "assets/pieces/bK.png"}
    };

    for (const auto& [name, path] : files) {
        sf::Texture tex;
        if (tex.loadFromFile(path)) {
            textures[name] = tex;
        }
    }
}

void PieceRenderer::render(sf::RenderWindow& window, const chess::Board& board) {
    float cellSize = 800.0f / 8.0f;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const auto& piece = board.getPiece({r, c});
            if (!piece.isEmpty()) {
                std::string name = (piece.getColor() == chess::Color::White ? "w" : "b");
                switch (piece.getType()) {
                    case chess::PieceType::Pawn:   name += "P"; break;
                    case chess::PieceType::Rook:   name += "R"; break;
                    case chess::PieceType::Knight: name += "N"; break;
                    case chess::PieceType::Bishop: name += "B"; break;
                    case chess::PieceType::Queen:  name += "Q"; break;
                    case chess::PieceType::King:   name += "K"; break;
                    default: break;
                }

                if (textures.count(name)) {
                    sf::Sprite sprite(textures[name]);
                    sprite.setOrigin(textures[name].getSize().x / 2.0f, textures[name].getSize().y / 2.0f);
                    sprite.setPosition(c * cellSize + cellSize / 2.0f, r * cellSize + cellSize / 2.0f);
                    // Scale sprite to fit cellSize
                    float scale = (cellSize * 0.8f) / textures[name].getSize().x;
                    sprite.setScale(scale, scale);
                    window.draw(sprite);
                }
            }
        }
    }
}

} // namespace ui
