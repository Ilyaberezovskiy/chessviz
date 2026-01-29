#include "board.h"

namespace chess {

Board::Board() {
    reset();
}

const Piece& Board::getPiece(Position pos) const {
    return squares[pos.row][pos.col];
}

void Board::setPiece(Position pos, Piece piece) {
    piece.setPosition(pos);
    squares[pos.row][pos.col] = piece;
}

void Board::clearSquare(Position pos) {
    squares[pos.row][pos.col] = Piece();
}

bool Board::isSquareOccupied(Position pos) const {
    return !squares[pos.row][pos.col].isEmpty();
}

bool Board::isValidPosition(Position pos) const {
    return pos.row >= 0 && pos.row < 8 && pos.col >= 0 && pos.col < 8;
}

void Board::reset() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            squares[r][c] = Piece();
        }
    }

    // Set up pieces
    auto setupRow = [&](int row, Color color) {
        setPiece({row, 0}, Piece(PieceType::Rook, color));
        setPiece({row, 1}, Piece(PieceType::Knight, color));
        setPiece({row, 2}, Piece(PieceType::Bishop, color));
        setPiece({row, 3}, Piece(PieceType::Queen, color));
        setPiece({row, 4}, Piece(PieceType::King, color));
        setPiece({row, 5}, Piece(PieceType::Bishop, color));
        setPiece({row, 6}, Piece(PieceType::Knight, color));
        setPiece({row, 7}, Piece(PieceType::Rook, color));
    };

    auto setupPawns = [&](int row, Color color) {
        for (int c = 0; c < 8; ++c) {
            setPiece({row, c}, Piece(PieceType::Pawn, color));
        }
    };

    setupRow(0, Color::Black);
    setupPawns(1, Color::Black);
    setupPawns(6, Color::White);
    setupRow(7, Color::White);
}

} // namespace chess
