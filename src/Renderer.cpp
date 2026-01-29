#include "Renderer.h"
#include <cmath>

Renderer::Renderer(sf::RenderWindow& win) : window(win) {
    squareSize = 80.0f;
    boardOffsetX = 50.0f;
    boardOffsetY = 50.0f;
    
    lightSquareColor = sf::Color(240, 217, 181);
    darkSquareColor = sf::Color(181, 136, 99);
    highlightColor = sf::Color(186, 202, 68, 150);
    lastMoveColor = sf::Color(205, 210, 106, 150);
    legalMoveColor = sf::Color(100, 100, 100, 100);
}

void Renderer::render(const GameState& gameState, const Position& selectedPiece,
                     const std::vector<Position>& legalMoves) {
    window.clear(sf::Color(49, 46, 43));
    
    drawBoard();
    drawHighlights(selectedPiece, legalMoves, gameState.getLastMove());
    drawPieces(gameState.getBoard());
    drawUI(gameState);
    
    window.display();
}

Position Renderer::screenToBoard(int x, int y) const {
    int col = static_cast<int>((x - boardOffsetX) / squareSize);
    int row = static_cast<int>((y - boardOffsetY) / squareSize);
    
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        return Position(row, col);
    }
    
    return Position();
}

void Renderer::drawBoard() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
            square.setPosition(boardOffsetX + col * squareSize, boardOffsetY + row * squareSize);
            
            if ((row + col) % 2 == 0) {
                square.setFillColor(lightSquareColor);
            } else {
                square.setFillColor(darkSquareColor);
            }
            
            window.draw(square);
        }
    }
    
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        font.loadFromFile("/System/Library/Fonts/Helvetica.ttc");
    }
    
    for (int i = 0; i < 8; i++) {
        sf::Text fileLabel;
        fileLabel.setFont(font);
        fileLabel.setString(std::string(1, 'a' + i));
        fileLabel.setCharacterSize(16);
        fileLabel.setFillColor(sf::Color(200, 200, 200));
        fileLabel.setPosition(boardOffsetX + i * squareSize + squareSize / 2 - 5,
                             boardOffsetY + 8 * squareSize + 5);
        window.draw(fileLabel);
        
        sf::Text rankLabel;
        rankLabel.setFont(font);
        rankLabel.setString(std::to_string(8 - i));
        rankLabel.setCharacterSize(16);
        rankLabel.setFillColor(sf::Color(200, 200, 200));
        rankLabel.setPosition(boardOffsetX - 25, boardOffsetY + i * squareSize + squareSize / 2 - 8);
        window.draw(rankLabel);
    }
}

void Renderer::drawPieces(const ChessBoard& board) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Position pos(row, col);
            const Piece& piece = board.getPiece(pos);
            if (!piece.isEmpty()) {
                drawPiece(piece, pos);
            }
        }
    }
}

void Renderer::drawHighlights(const Position& selectedPiece, const std::vector<Position>& legalMoves,
                              const Move& lastMove) {
    if (lastMove.from.isValid() && lastMove.to.isValid()) {
        sf::RectangleShape highlight(sf::Vector2f(squareSize, squareSize));
        
        highlight.setPosition(boardOffsetX + lastMove.from.col * squareSize,
                            boardOffsetY + lastMove.from.row * squareSize);
        highlight.setFillColor(lastMoveColor);
        window.draw(highlight);
        
        highlight.setPosition(boardOffsetX + lastMove.to.col * squareSize,
                            boardOffsetY + lastMove.to.row * squareSize);
        window.draw(highlight);
    }
    
    if (selectedPiece.isValid()) {
        sf::RectangleShape highlight(sf::Vector2f(squareSize, squareSize));
        highlight.setPosition(boardOffsetX + selectedPiece.col * squareSize,
                            boardOffsetY + selectedPiece.row * squareSize);
        highlight.setFillColor(highlightColor);
        window.draw(highlight);
    }
    
    for (const Position& pos : legalMoves) {
        sf::CircleShape dot(squareSize / 6);
        dot.setFillColor(legalMoveColor);
        dot.setPosition(boardOffsetX + pos.col * squareSize + squareSize / 3,
                       boardOffsetY + pos.row * squareSize + squareSize / 3);
        window.draw(dot);
    }
}

void Renderer::drawPiece(const Piece& piece, const Position& pos) {
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        font.loadFromFile("/System/Library/Fonts/Helvetica.ttc");
    }
    
    sf::Text text;
    text.setFont(font);
    text.setString(getPieceSymbol(piece));
    text.setCharacterSize(static_cast<unsigned int>(squareSize * 0.7f));
    
    if (piece.getColor() == Color::WHITE) {
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2);
    } else {
        text.setFillColor(sf::Color(50, 50, 50));
        text.setOutlineColor(sf::Color::White);
        text.setOutlineThickness(1);
    }
    
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f,
                  textBounds.top + textBounds.height / 2.0f);
    text.setPosition(boardOffsetX + pos.col * squareSize + squareSize / 2.0f,
                    boardOffsetY + pos.row * squareSize + squareSize / 2.0f);
    
    window.draw(text);
}

void Renderer::drawUI(const GameState& gameState) {
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        font.loadFromFile("/System/Library/Fonts/Helvetica.ttc");
    }
    
    std::string statusText;
    Color currentPlayer = gameState.getCurrentPlayer();
    GameStatus status = gameState.getStatus();
    
    if (status == GameStatus::CHECKMATE) {
        statusText = (currentPlayer == Color::WHITE) ? "Black wins by checkmate!" : "White wins by checkmate!";
    } else if (status == GameStatus::STALEMATE) {
        statusText = "Draw by stalemate";
    } else if (status == GameStatus::DRAW) {
        statusText = "Draw";
    } else if (status == GameStatus::CHECK) {
        statusText = (currentPlayer == Color::WHITE) ? "White is in check" : "Black is in check";
    } else {
        statusText = (currentPlayer == Color::WHITE) ? "White to move" : "Black to move";
    }
    
    sf::Text statusLabel;
    statusLabel.setFont(font);
    statusLabel.setString(statusText);
    statusLabel.setCharacterSize(24);
    statusLabel.setFillColor(sf::Color::White);
    statusLabel.setPosition(boardOffsetX + 8 * squareSize + 30, boardOffsetY);
    window.draw(statusLabel);
    
    drawButton("New Game", boardOffsetX + 8 * squareSize + 30, boardOffsetY + 60, 150, 40);
    drawButton("Undo", boardOffsetX + 8 * squareSize + 30, boardOffsetY + 110, 150, 40);
}

void Renderer::drawButton(const std::string& text, float x, float y, float width, float height) {
    sf::RectangleShape button(sf::Vector2f(width, height));
    button.setPosition(x, y);
    button.setFillColor(sf::Color(70, 70, 70));
    button.setOutlineColor(sf::Color(150, 150, 150));
    button.setOutlineThickness(2);
    window.draw(button);
    
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        font.loadFromFile("/System/Library/Fonts/Helvetica.ttc");
    }
    
    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(18);
    buttonText.setFillColor(sf::Color::White);
    
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f,
                        textBounds.top + textBounds.height / 2.0f);
    buttonText.setPosition(x + width / 2.0f, y + height / 2.0f);
    
    window.draw(buttonText);
}

std::string Renderer::getPieceSymbol(const Piece& piece) const {
    switch (piece.getType()) {
        case PieceType::KING:   return "♔";
        case PieceType::QUEEN:  return "♕";
        case PieceType::ROOK:   return "♖";
        case PieceType::BISHOP: return "♗";
        case PieceType::KNIGHT: return "♘";
        case PieceType::PAWN:   return "♙";
        default: return "";
    }
}
