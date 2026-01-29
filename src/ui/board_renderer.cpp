#include "board_renderer.h"

namespace ui {

BoardRenderer::BoardRenderer() {
    lightColor = sf::Color(238, 238, 210);
    darkColor = sf::Color(118, 150, 86);
    highlightColor = sf::Color(186, 202, 68, 128);
    moveColor = sf::Color(0, 0, 0, 26);
    lastMoveColor = sf::Color(246, 246, 105, 128);
    checkColor = sf::Color(255, 0, 0, 128);

    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        // Handle error if needed
    }
}

void BoardRenderer::render(sf::RenderWindow& window, const chess::Board& board,
                            chess::Position selectedPos, const std::vector<chess::Move>& validMoves,
                            const std::vector<chess::Move>& history,
                            chess::GameStatus status, chess::Color currentPlayer) {
    drawGrid(window);
    drawHighlights(window, selectedPos, validMoves, history, status, currentPlayer, board);
    drawCoordinates(window);
}

void BoardRenderer::drawGrid(sf::RenderWindow& window) {
    float cellSize = 800.0f / 8.0f;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            sf::RectangleShape square(sf::Vector2f(cellSize, cellSize));
            square.setPosition(c * cellSize, r * cellSize);
            square.setFillColor((r + c) % 2 == 0 ? lightColor : darkColor);
            window.draw(square);
        }
    }
}

void BoardRenderer::drawCoordinates(sf::RenderWindow& window) {
    float cellSize = 800.0f / 8.0f;
    for (int i = 0; i < 8; ++i) {
        // Files (a-h)
        sf::Text fileText(std::string(1, 'a' + i), font, 16);
        fileText.setFillColor((i % 2 == 0) ? darkColor : lightColor);
        fileText.setPosition(i * cellSize + cellSize - 15, 800 - 20);
        window.draw(fileText);

        // Ranks (1-8)
        sf::Text rankText(std::to_string(8 - i), font, 16);
        rankText.setFillColor((i % 2 == 0) ? lightColor : darkColor);
        rankText.setPosition(5, i * cellSize + 5);
        window.draw(rankText);
    }
}

void BoardRenderer::drawHighlights(sf::RenderWindow& window, chess::Position selectedPos,
                                   const std::vector<chess::Move>& validMoves,
                                   const std::vector<chess::Move>& history,
                                   chess::GameStatus status, chess::Color currentPlayer,
                                   const chess::Board& board) {
    float cellSize = 800.0f / 8.0f;

    // Check highlight
    if (status == chess::GameStatus::Check || status == chess::GameStatus::Checkmate) {
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                const auto& p = board.getPiece({r, c});
                if (p.getType() == chess::PieceType::King && p.getColor() == currentPlayer) {
                    sf::RectangleShape checkRect(sf::Vector2f(cellSize, cellSize));
                    checkRect.setPosition(c * cellSize, r * cellSize);
                    checkRect.setFillColor(checkColor);
                    window.draw(checkRect);
                }
            }
        }
    }

    // Last move highlight
    if (!history.empty()) {
        const auto& lastMove = history.back();
        sf::RectangleShape fromRect(sf::Vector2f(cellSize, cellSize));
        fromRect.setPosition(lastMove.from.col * cellSize, lastMove.from.row * cellSize);
        fromRect.setFillColor(lastMoveColor);
        window.draw(fromRect);

        sf::RectangleShape toRect(sf::Vector2f(cellSize, cellSize));
        toRect.setPosition(lastMove.to.col * cellSize, lastMove.to.row * cellSize);
        toRect.setFillColor(lastMoveColor);
        window.draw(toRect);
    }

    // Selected piece highlight
    if (selectedPos.row != -1) {
        sf::RectangleShape selectedRect(sf::Vector2f(cellSize, cellSize));
        selectedRect.setPosition(selectedPos.col * cellSize, selectedPos.row * cellSize);
        selectedRect.setFillColor(highlightColor);
        window.draw(selectedRect);

        // Valid moves
        for (const auto& move : validMoves) {
            sf::CircleShape moveCircle(cellSize / 6.0f);
            moveCircle.setOrigin(moveCircle.getRadius(), moveCircle.getRadius());
            moveCircle.setPosition(move.to.col * cellSize + cellSize / 2.0f, move.to.row * cellSize + cellSize / 2.0f);
            moveCircle.setFillColor(moveColor);
            window.draw(moveCircle);
        }
    }
}

} // namespace ui
