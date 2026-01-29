#include "window.h"

namespace ui {

Window::Window() : window(sf::VideoMode(1100, 800), "Chess Game") {
    window.setFramerateLimit(60);
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        // Handle error
    }
}

void Window::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Window::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        inputHandler.handleInput(window, event, game);
    }
}

void Window::update() {
    // Game logic updates if any (most handled by InputHandler)
}

void Window::render() {
    window.clear(sf::Color(49, 46, 43)); // Dark background like chess.com
    
    boardRenderer.render(window, game.getBoard(), 
                         inputHandler.getSelectedPosition(), 
                         inputHandler.getValidMoves(),
                         game.getHistory(),
                         game.getStatus(),
                         game.getCurrentPlayer());
    pieceRenderer.render(window, game.getBoard());
    
    drawPanel();
    
    window.display();
}

void Window::drawPanel() {
    sf::RectangleShape panel(sf::Vector2f(300, 800));
    panel.setPosition(800, 0);
    panel.setFillColor(sf::Color(38, 37, 34));
    window.draw(panel);

    sf::Text turnText;
    turnText.setFont(font);
    turnText.setCharacterSize(24);
    turnText.setFillColor(sf::Color::White);
    turnText.setPosition(820, 20);
    turnText.setString("Turn: " + std::string(game.getCurrentPlayer() == chess::Color::White ? "White" : "Black"));
    window.draw(turnText);

    sf::Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(24);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition(820, 60);
    std::string statusStr = "Status: ";
    switch (game.getStatus()) {
        case chess::GameStatus::Normal: statusStr += "Normal"; break;
        case chess::GameStatus::Check: statusStr += "Check"; break;
        case chess::GameStatus::Checkmate: statusStr += "Checkmate!"; break;
        case chess::GameStatus::Stalemate: statusStr += "Stalemate"; break;
        case chess::GameStatus::Draw: statusStr += "Draw"; break;
    }
    statusText.setString(statusStr);
    window.draw(statusText);

    sf::Text helpText;
    helpText.setFont(font);
    helpText.setCharacterSize(16);
    helpText.setFillColor(sf::Color(150, 150, 150));
    helpText.setPosition(820, 700);
    helpText.setString("R: Reset  U: Undo");
    window.draw(helpText);

    // Draw history
    sf::Text historyTitle;
    historyTitle.setFont(font);
    historyTitle.setCharacterSize(20);
    historyTitle.setFillColor(sf::Color::White);
    historyTitle.setPosition(820, 120);
    historyTitle.setString("History:");
    window.draw(historyTitle);

    const auto& history = game.getHistory();
    int start = std::max(0, (int)history.size() - 20);
    for (int i = start; i < history.size(); ++i) {
        sf::Text moveText;
        moveText.setFont(font);
        moveText.setCharacterSize(16);
        moveText.setFillColor(sf::Color::White);
        moveText.setPosition(820 + (i % 2 == 0 ? 0 : 100), 150 + (i - start) / 2 * 25);
        
        std::string moveStr = "";
        if (i % 2 == 0) moveStr += std::to_string(i/2 + 1) + ". ";
        moveStr += game.getMoveNotation(history[i]);
        moveText.setString(moveStr);
        window.draw(moveText);
    }
}

} // namespace ui
