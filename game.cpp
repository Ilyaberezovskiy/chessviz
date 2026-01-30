#include "game.h"
#include "ai.h"
#include <iostream>
#include <string>
#include <algorithm>

Game::Game()
    : whitePlayer_("Белые", Color::White)
    , blackPlayer_("Чёрные", Color::Black)
    , currentTurn_(Color::White)
    , aiEnabled_(false)
    , aiColor_(Color::Black)
    , flipped_(false)
{
    board_.setupInitialPosition();
}

Game::Game(bool aiEnabled, Color aiColor)
    : whitePlayer_(aiColor == Color::White ? "Компьютер" : "Игрок", Color::White)
    , blackPlayer_(aiColor == Color::Black ? "Компьютер" : "Игрок", Color::Black)
    , currentTurn_(Color::White)
    , aiEnabled_(aiEnabled)
    , aiColor_(aiColor)
    , flipped_(aiColor == Color::White)
{
    board_.setupInitialPosition();
}

Player& Game::getCurrentPlayer() {
    return currentTurn_ == Color::White ? whitePlayer_ : blackPlayer_;
}

void Game::switchTurn() {
    currentTurn_ = oppositeColor(currentTurn_);
}

bool Game::isAiTurn() const {
    return aiEnabled_ && currentTurn_ == aiColor_;
}

void Game::run() {
    std::cout << "=== Шахматы ===\n";
    std::cout << "Формат хода: e2e4 или e2 e4\n";
    std::cout << "Превращение: e7e8q (q/r/b/n)\n";
    std::cout << "Выход: quit, exit или q\n";

    while (true) {
        board_.display(flipped_);

        // Проверка состояния игры
        GameState state = board_.evaluateGameState(currentTurn_);

        switch (state) {
            case GameState::Checkmate:
                std::cout << "Мат! " << (currentTurn_ == Color::White ? "Чёрные" : "Белые")
                          << " побеждают!\n";
                return;
            case GameState::Stalemate:
                std::cout << "Пат! Ничья.\n";
                return;
            case GameState::DrawBy50Moves:
                std::cout << "Ничья по правилу 50 ходов.\n";
                return;
            case GameState::DrawByRepetition:
                std::cout << "Ничья по троекратному повторению позиции.\n";
                return;
            case GameState::InProgress:
                break;
        }

        // Сообщение о шахе
        if (board_.isInCheck(currentTurn_)) {
            std::cout << "Шах!\n";
        }

        // Ход AI
        if (isAiTurn()) {
            std::string colorName = (currentTurn_ == Color::White) ? "белые" : "чёрные";
            std::cout << "Компьютер (" << colorName << ") думает...\n";
            Move aiMove = findBestMove(board_, currentTurn_);
            std::cout << "Компьютер ходит: " << aiMove.toString() << "\n";
            board_.makeMove(aiMove);
            switchTurn();
            continue;
        }

        // Ход игрока
        std::string colorName = (currentTurn_ == Color::White) ? "белые" : "чёрные";
        std::cout << "Ваш ход (" << colorName << "): ";

        std::string input;
        if (!std::getline(std::cin, input)) {
            std::cout << "\n";
            return;
        }

        // Удаляем пробелы по краям
        auto ltrim = input.find_first_not_of(" \t\r\n");
        if (ltrim == std::string::npos) continue;
        input = input.substr(ltrim);
        auto rtrim = input.find_last_not_of(" \t\r\n");
        if (rtrim != std::string::npos) input = input.substr(0, rtrim + 1);

        if (input.empty()) continue;

        // Команды выхода
        if (input == "quit" || input == "exit" || input == "q") {
            std::cout << "Игра завершена.\n";
            return;
        }

        // Парсинг хода
        auto moveOpt = parseMove(input);
        if (!moveOpt.has_value()) {
            std::cout << "Неверный формат хода. Используйте: e2e4 или e2 e4\n";
            continue;
        }

        Move move = moveOpt.value();

        // Проверка: если пешка идёт на последнюю горизонталь без указания превращения
        const auto* piece = board_.getPiece(move.from);
        if (piece && piece->type == PieceType::Pawn && piece->color == currentTurn_) {
            int promotionRow = (currentTurn_ == Color::White) ? 7 : 0;
            if (move.to.row == promotionRow && move.promotion == '\0') {
                // Запрашиваем фигуру для превращения
                std::cout << "Выберите фигуру для превращения (q - ферзь, r - ладья, b - слон, n - конь): ";
                std::string promoInput;
                if (!std::getline(std::cin, promoInput)) {
                    std::cout << "\n";
                    return;
                }
                // Берём первый символ
                char promo = '\0';
                for (char ch : promoInput) {
                    if (ch == 'q' || ch == 'r' || ch == 'b' || ch == 'n' ||
                        ch == 'Q' || ch == 'R' || ch == 'B' || ch == 'N') {
                        promo = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
                        break;
                    }
                }
                if (promo == '\0') {
                    promo = 'q'; // по умолчанию — ферзь
                    std::cout << "Выбран ферзь по умолчанию.\n";
                }
                move.promotion = promo;
            }
        }

        // Проверка легальности хода
        if (!board_.isMoveLegal(move, currentTurn_)) {
            std::cout << "Нелегальный ход!\n";
            continue;
        }

        // Выполнение хода
        board_.makeMove(move);
        switchTurn();
    }
}
