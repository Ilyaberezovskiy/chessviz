#include "piece_renderer.h"

namespace ui {

void PieceRenderer::render(sf::RenderWindow& window, const chess::Board& board) {
    float cellSize = 800.0f / 8.0f;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const auto& piece = board.getPiece({r, c});
            if (!piece.isEmpty()) {
                bool isWhite = (piece.getColor() == chess::Color::White);
                float x = c * cellSize + cellSize / 2.0f;
                float y = r * cellSize + cellSize / 2.0f;
                
                switch (piece.getType()) {
                    case chess::PieceType::Pawn:
                        drawPawn(window, x, y, isWhite);
                        break;
                    case chess::PieceType::Knight:
                        drawKnight(window, x, y, isWhite);
                        break;
                    case chess::PieceType::Bishop:
                        drawBishop(window, x, y, isWhite);
                        break;
                    case chess::PieceType::Rook:
                        drawRook(window, x, y, isWhite);
                        break;
                    case chess::PieceType::Queen:
                        drawQueen(window, x, y, isWhite);
                        break;
                    case chess::PieceType::King:
                        drawKing(window, x, y, isWhite);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void PieceRenderer::drawPawn(sf::RenderTarget& target, float x, float y, bool isWhite) {
    sf::Color color = getColorForPiece(isWhite);
    float radius = 10.0f;
    
    // Основной круг
    sf::CircleShape pawn(radius);
    pawn.setPosition(x - radius, y - radius);
    pawn.setFillColor(color);
    pawn.setOutlineColor(sf::Color::Black);
    pawn.setOutlineThickness(2.0f);
    target.draw(pawn);
    
    // Треугольник сверху (маленький круг)
    sf::CircleShape top(radius * 0.6f);
    top.setPosition(x - radius * 0.6f, y - radius * 2.2f);
    top.setFillColor(color);
    top.setOutlineColor(sf::Color::Black);
    top.setOutlineThickness(1.5f);
    target.draw(top);
}

void PieceRenderer::drawKnight(sf::RenderTarget& target, float x, float y, bool isWhite) {
    sf::Color color = getColorForPiece(isWhite);
    float radius = 8.0f;
    
    // L-образная форма из двух кругов
    // Основной круг
    sf::CircleShape mainCircle(radius);
    mainCircle.setPosition(x - radius, y - radius);
    mainCircle.setFillColor(color);
    mainCircle.setOutlineColor(sf::Color::Black);
    mainCircle.setOutlineThickness(2.0f);
    target.draw(mainCircle);
    
    // Второй круг для L-формы
    sf::CircleShape secondCircle(radius * 0.8f);
    secondCircle.setPosition(x + radius * 0.2f, y - radius * 1.2f);
    secondCircle.setFillColor(color);
    secondCircle.setOutlineColor(sf::Color::Black);
    secondCircle.setOutlineThickness(2.0f);
    target.draw(secondCircle);
    
    // Треугольник сверху (голова коня)
    sf::CircleShape head(radius * 0.6f);
    head.setPosition(x - radius * 0.3f, y - radius * 2.5f);
    head.setFillColor(color);
    head.setOutlineColor(sf::Color::Black);
    head.setOutlineThickness(1.5f);
    target.draw(head);
}

void PieceRenderer::drawBishop(sf::RenderTarget& target, float x, float y, bool isWhite) {
    sf::Color color = getColorForPiece(isWhite);
    float radius = 11.0f;
    
    // Основной круг
    sf::CircleShape bishop(radius);
    bishop.setPosition(x - radius, y - radius);
    bishop.setFillColor(color);
    bishop.setOutlineColor(sf::Color::Black);
    bishop.setOutlineThickness(2.0f);
    target.draw(bishop);
    
    // Маленький круг сверху (капюшон)
    sf::CircleShape hood(radius * 0.6f);
    hood.setPosition(x - radius * 0.6f, y - radius * 2.2f);
    hood.setFillColor(color);
    hood.setOutlineColor(sf::Color::Black);
    hood.setOutlineThickness(1.5f);
    target.draw(hood);
}

void PieceRenderer::drawRook(sf::RenderTarget& target, float x, float y, bool isWhite) {
    sf::Color color = getColorForPiece(isWhite);
    float size = 12.0f;
    
    // Основной квадрат
    sf::RectangleShape rook(sf::Vector2f(size * 2, size * 2));
    rook.setPosition(x - size, y - size);
    rook.setFillColor(color);
    rook.setOutlineColor(sf::Color::Black);
    rook.setOutlineThickness(2.0f);
    target.draw(rook);
    
    // Зубцы сверху (три маленьких квадрата)
    for (int i = 0; i < 3; i++) {
        sf::RectangleShape notch(sf::Vector2f(size * 0.6f, size * 0.8f));
        notch.setPosition(x - size + i * size * 0.8f, y - size * 2);
        notch.setFillColor(color);
        notch.setOutlineColor(sf::Color::Black);
        notch.setOutlineThickness(1.5f);
        target.draw(notch);
    }
}

void PieceRenderer::drawQueen(sf::RenderTarget& target, float x, float y, bool isWhite) {
    sf::Color color = getColorForPiece(isWhite);
    float radius = 11.0f;
    
    // Основной круг
    sf::CircleShape queen(radius);
    queen.setPosition(x - radius, y - radius);
    queen.setFillColor(color);
    queen.setOutlineColor(sf::Color::Black);
    queen.setOutlineThickness(2.0f);
    target.draw(queen);
    
    // Корона (5 маленьких кругов сверху)
    float crownRadius = radius * 0.5f;
    for (int i = 0; i < 5; i++) {
        sf::CircleShape crown(crownRadius);
        crown.setPosition(
            x - radius + (i * radius * 0.8f) - crownRadius,
            y - radius * 2.2f - crownRadius
        );
        crown.setFillColor(color);
        crown.setOutlineColor(sf::Color::Black);
        crown.setOutlineThickness(1.5f);
        target.draw(crown);
    }
}

void PieceRenderer::drawKing(sf::RenderTarget& target, float x, float y, bool isWhite) {
    sf::Color color = getColorForPiece(isWhite);
    float size = 10.0f;
    
    // Основной квадрат (толще)
    sf::RectangleShape king(sf::Vector2f(size * 2.2f, size * 2));
    king.setPosition(x - size * 1.1f, y - size);
    king.setFillColor(color);
    king.setOutlineColor(sf::Color::Black);
    king.setOutlineThickness(2.0f);
    target.draw(king);
    
    // Крест сверху (вертикальная и горизонтальная линии)
    // Вертикальная линия
    sf::RectangleShape vertical(sf::Vector2f(size * 0.6f, size * 2.5f));
    vertical.setPosition(x - size * 0.3f, y - size * 2.2f);
    vertical.setFillColor(color);
    vertical.setOutlineColor(sf::Color::Black);
    vertical.setOutlineThickness(1.5f);
    target.draw(vertical);
    
    // Горизонтальная линия
    sf::RectangleShape horizontal(sf::Vector2f(size * 1.5f, size * 0.6f));
    horizontal.setPosition(x - size * 0.75f, y - size * 1.5f);
    horizontal.setFillColor(color);
    horizontal.setOutlineColor(sf::Color::Black);
    horizontal.setOutlineThickness(1.5f);
    target.draw(horizontal);
}

sf::Color PieceRenderer::getColorForPiece(bool isWhite) {
    return isWhite ? sf::Color(240, 240, 240) : sf::Color::Black;
}

} // namespace ui