#ifndef BOARD_H
#define BOARD_H

#include "pieces.h"
#include <memory>
#include <optional>
#include <string>
#include <vector>

enum class GameState {
    InProgress,
    Checkmate,
    Stalemate,
    DrawBy50Moves,
    DrawByRepetition
};

class Board {
public:
    Board();

    void setupInitialPosition();
    void display(bool flipped = false) const;

    // Доступ к фигурам
    const Piece* getPiece(const Square& sq) const;

    // Права рокировки
    bool canCastleKingside(Color c) const;
    bool canCastleQueenside(Color c) const;

    // En passant
    std::optional<Square> getEnPassantTarget() const { return enPassantTarget_; }

    // Проверка атаки
    bool isSquareAttackedBy(const Square& sq, Color byColor) const;
    bool isInCheck(Color side) const;
    Square findKing(Color side) const;

    // Глубокая копия для проверки легальности
    Board copyForTest() const;

    // Легальность хода
    bool isMoveLegal(const Move& move, Color side) const;
    std::vector<Move> getLegalMoves(Color side) const;

    // Выполнение хода (без проверки легальности — должна быть выполнена заранее)
    void makeMove(const Move& move);

    // FEN-подобный ключ позиции для троекратного повторения
    std::string getPositionKey(Color sideToMove) const;

    // Оценка состояния игры
    GameState evaluateGameState(Color sideToMove);

private:
    std::unique_ptr<Piece> grid_[8][8];

    // Права рокировки
    bool whiteKingsideCastle_ = true;
    bool whiteQueensideCastle_ = true;
    bool blackKingsideCastle_ = true;
    bool blackQueensideCastle_ = true;

    std::optional<Square> enPassantTarget_;
    int halfmoveClock_ = 0;
    std::vector<std::string> positionHistory_;

    void placePiece(int row, int col, std::unique_ptr<Piece> piece);
};

#endif
