#ifndef RENDERER_H
#define RENDERER_H

#include "GameState.h"
#include "Types.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>

class Renderer {
private:
    sf::RenderWindow& window;
    std::map<std::string, sf::Font> fonts;
    float squareSize;
    float boardOffsetX;
    float boardOffsetY;
    
    sf::Color lightSquareColor;
    sf::Color darkSquareColor;
    sf::Color highlightColor;
    sf::Color lastMoveColor;
    sf::Color legalMoveColor;

public:
    Renderer(sf::RenderWindow& win);
    
    void render(const GameState& gameState, const Position& selectedPiece,
                const std::vector<Position>& legalMoves);
    
    Position screenToBoard(int x, int y) const;
    
private:
    void drawBoard();
    void drawPieces(const ChessBoard& board);
    void drawHighlights(const Position& selectedPiece, const std::vector<Position>& legalMoves,
                       const Move& lastMove);
    void drawPiece(const Piece& piece, const Position& pos);
    void drawUI(const GameState& gameState);
    void drawButton(const std::string& text, float x, float y, float width, float height);
    
    sf::CircleShape createPieceShape(const Piece& piece, const Position& pos);
    std::string getPieceSymbol(const Piece& piece) const;
};

#endif
