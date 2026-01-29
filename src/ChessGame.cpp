#include "ChessGame.h"

ChessGame::ChessGame() 
    : window(sf::VideoMode(900, 750), "Chess Game", sf::Style::Titlebar | sf::Style::Close),
      renderer(window),
      pieceSelected(false),
      promotionActive(false) {
    window.setFramerateLimit(60);
}

void ChessGame::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void ChessGame::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                handleClick(event.mouseButton.x, event.mouseButton.y);
            }
        }
    }
}

void ChessGame::handleClick(int x, int y) {
    if (promotionActive) {
        PieceType type;
        if (isPromotionChoiceClicked(x, y, type)) {
            handlePromotion(type);
        }
        return;
    }
    
    if (isNewGameButtonClicked(x, y)) {
        gameState.reset();
        pieceSelected = false;
        legalMoves.clear();
        selectedPiece = Position();
        return;
    }
    
    if (isUndoButtonClicked(x, y)) {
        gameState.undoMove();
        pieceSelected = false;
        legalMoves.clear();
        selectedPiece = Position();
        return;
    }
    
    Position clickedPos = renderer.screenToBoard(x, y);
    
    if (!clickedPos.isValid()) {
        pieceSelected = false;
        legalMoves.clear();
        selectedPiece = Position();
        return;
    }
    
    if (pieceSelected) {
        bool moveMade = false;
        for (const Position& legalMove : legalMoves) {
            if (legalMove == clickedPos) {
                Move move(selectedPiece, clickedPos);
                
                const Piece& piece = gameState.getBoard().getPiece(selectedPiece);
                if (piece.getType() == PieceType::PAWN) {
                    if ((piece.getColor() == Color::WHITE && clickedPos.row == 0) ||
                        (piece.getColor() == Color::BLACK && clickedPos.row == 7)) {
                        promotionActive = true;
                        promotionSquare = clickedPos;
                        return;
                    }
                    
                    Position enPassant = gameState.getBoard().getEnPassantTarget();
                    if (enPassant.isValid() && clickedPos == enPassant) {
                        move.isEnPassant = true;
                    }
                }
                
                if (piece.getType() == PieceType::KING) {
                    if (abs(clickedPos.col - selectedPiece.col) == 2) {
                        move.isCastling = true;
                    }
                }
                
                if (!gameState.getBoard().getPiece(clickedPos).isEmpty()) {
                    move.isCapture = true;
                }
                
                moveMade = gameState.makeMove(move);
                break;
            }
        }
        
        if (moveMade || gameState.getBoard().getPiece(clickedPos).getColor() != gameState.getCurrentPlayer()) {
            pieceSelected = false;
            legalMoves.clear();
            selectedPiece = Position();
        } else {
            const Piece& piece = gameState.getBoard().getPiece(clickedPos);
            if (!piece.isEmpty() && piece.getColor() == gameState.getCurrentPlayer()) {
                selectedPiece = clickedPos;
                legalMoves = gameState.getLegalMovesForPiece(clickedPos);
                pieceSelected = !legalMoves.empty();
            }
        }
    } else {
        const Piece& piece = gameState.getBoard().getPiece(clickedPos);
        if (!piece.isEmpty() && piece.getColor() == gameState.getCurrentPlayer()) {
            selectedPiece = clickedPos;
            legalMoves = gameState.getLegalMovesForPiece(clickedPos);
            pieceSelected = !legalMoves.empty();
        }
    }
}

void ChessGame::handlePromotion(PieceType type) {
    if (!promotionActive) return;
    
    Move move(selectedPiece, promotionSquare);
    move.isPromotion = true;
    move.promotionType = type;
    
    gameState.makeMove(move);
    
    promotionActive = false;
    pieceSelected = false;
    legalMoves.clear();
    selectedPiece = Position();
}

void ChessGame::update() {
}

void ChessGame::render() {
    if (promotionActive) {
        window.clear(sf::Color(49, 46, 43));
        
        renderer.render(gameState, selectedPiece, legalMoves);
        
        sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        
        float dialogWidth = 400;
        float dialogHeight = 200;
        float dialogX = (window.getSize().x - dialogWidth) / 2;
        float dialogY = (window.getSize().y - dialogHeight) / 2;
        
        sf::RectangleShape dialog(sf::Vector2f(dialogWidth, dialogHeight));
        dialog.setPosition(dialogX, dialogY);
        dialog.setFillColor(sf::Color(50, 50, 50));
        dialog.setOutlineColor(sf::Color::White);
        dialog.setOutlineThickness(3);
        window.draw(dialog);
        
        sf::Font font;
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            font.loadFromFile("/System/Library/Fonts/Helvetica.ttc");
        }
        
        sf::Text title;
        title.setFont(font);
        title.setString("Promote to:");
        title.setCharacterSize(24);
        title.setFillColor(sf::Color::White);
        title.setPosition(dialogX + dialogWidth / 2 - 60, dialogY + 20);
        window.draw(title);
        
        std::string pieces[] = {"♕", "♖", "♗", "♘"};
        float buttonY = dialogY + 70;
        float buttonSpacing = 80;
        float startX = dialogX + 50;
        
        for (int i = 0; i < 4; i++) {
            sf::RectangleShape button(sf::Vector2f(60, 60));
            button.setPosition(startX + i * buttonSpacing, buttonY);
            button.setFillColor(sf::Color(70, 70, 70));
            button.setOutlineColor(sf::Color::White);
            button.setOutlineThickness(2);
            window.draw(button);
            
            sf::Text pieceText;
            pieceText.setFont(font);
            pieceText.setString(pieces[i]);
            pieceText.setCharacterSize(40);
            pieceText.setFillColor(sf::Color::White);
            
            sf::FloatRect textBounds = pieceText.getLocalBounds();
            pieceText.setOrigin(textBounds.left + textBounds.width / 2.0f,
                              textBounds.top + textBounds.height / 2.0f);
            pieceText.setPosition(startX + i * buttonSpacing + 30, buttonY + 30);
            window.draw(pieceText);
        }
        
        window.display();
    } else {
        renderer.render(gameState, selectedPiece, legalMoves);
    }
}

bool ChessGame::isNewGameButtonClicked(int x, int y) const {
    float buttonX = 50 + 8 * 80 + 30;
    float buttonY = 50 + 60;
    float buttonWidth = 150;
    float buttonHeight = 40;
    
    return x >= buttonX && x <= buttonX + buttonWidth &&
           y >= buttonY && y <= buttonY + buttonHeight;
}

bool ChessGame::isUndoButtonClicked(int x, int y) const {
    float buttonX = 50 + 8 * 80 + 30;
    float buttonY = 50 + 110;
    float buttonWidth = 150;
    float buttonHeight = 40;
    
    return x >= buttonX && x <= buttonX + buttonWidth &&
           y >= buttonY && y <= buttonY + buttonHeight;
}

bool ChessGame::isPromotionChoiceClicked(int x, int y, PieceType& outType) const {
    float dialogWidth = 400;
    float dialogHeight = 200;
    float dialogX = (window.getSize().x - dialogWidth) / 2;
    float dialogY = (window.getSize().y - dialogHeight) / 2;
    
    float buttonY = dialogY + 70;
    float buttonSpacing = 80;
    float startX = dialogX + 50;
    
    for (int i = 0; i < 4; i++) {
        float buttonX = startX + i * buttonSpacing;
        if (x >= buttonX && x <= buttonX + 60 &&
            y >= buttonY && y <= buttonY + 60) {
            switch (i) {
                case 0: outType = PieceType::QUEEN; break;
                case 1: outType = PieceType::ROOK; break;
                case 2: outType = PieceType::BISHOP; break;
                case 3: outType = PieceType::KNIGHT; break;
            }
            return true;
        }
    }
    
    return false;
}
