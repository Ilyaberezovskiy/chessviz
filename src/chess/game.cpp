#include "game.h"
#include <algorithm>

namespace chess {

Game::Game() {
    reset();
}

void Game::reset() {
    board.reset();
    currentPlayer = Color::White;
    status = GameStatus::Normal;
    history.clear();
    boardHistory.clear();
    boardHistory.push_back(board);
}

bool Game::makeMove(Position from, Position to, PieceType promotion) {
    if (status == GameStatus::Checkmate || status == GameStatus::Stalemate || status == GameStatus::Draw) {
        return false;
    }

    std::vector<Move> validMoves = Rules::getValidMoves(board, from, history);
    auto it = std::find_if(validMoves.begin(), validMoves.end(), [&](const Move& m) {
        return m.to == to && (m.type != MoveType::Promotion || m.promotionType == promotion);
    });

    if (it == validMoves.end()) {
        return false;
    }

    Move move = *it;
    move.capturedPiece = board.getPiece(to);

    // Apply move
    if (move.type == MoveType::Castling) {
        int row = from.row;
        bool kingSide = (to.col == 6);
        board.clearSquare(from);
        board.setPiece(to, Piece(PieceType::King, currentPlayer));
        if (kingSide) {
            board.clearSquare({row, 7});
            board.setPiece({row, 5}, Piece(PieceType::Rook, currentPlayer));
        } else {
            board.clearSquare({row, 0});
            board.setPiece({row, 3}, Piece(PieceType::Rook, currentPlayer));
        }
    } else if (move.type == MoveType::EnPassant) {
        board.clearSquare(from);
        board.setPiece(to, Piece(PieceType::Pawn, currentPlayer));
        int captureRow = (currentPlayer == Color::White) ? to.row + 1 : to.row - 1;
        move.capturedPiece = board.getPiece({captureRow, to.col});
        board.clearSquare({captureRow, to.col});
    } else if (move.type == MoveType::Promotion) {
        board.clearSquare(from);
        board.setPiece(to, Piece(promotion, currentPlayer));
    } else {
        board.clearSquare(from);
        board.setPiece(to, Piece(board.getPiece(from).getType(), currentPlayer));
    }

    history.push_back(move);
    boardHistory.push_back(board);
    switchPlayer();
    updateStatus();

    return true;
}

void Game::undoMove() {
    if (history.empty()) return;

    history.pop_back();
    boardHistory.pop_back();
    board = boardHistory.back();
    switchPlayer();
    updateStatus();
}

void Game::switchPlayer() {
    currentPlayer = (currentPlayer == Color::White) ? Color::Black : Color::White;
}

void Game::updateStatus() {
    if (Rules::isCheckmate(board, currentPlayer, history)) {
        status = GameStatus::Checkmate;
    } else if (Rules::isStalemate(board, currentPlayer, history)) {
        status = GameStatus::Stalemate;
    } else if (Rules::isKingInCheck(board, currentPlayer, history)) {
        status = GameStatus::Check;
    } else {
        status = GameStatus::Normal;
    }
    // TODO: Add other draw conditions if needed
}

std::string Game::getMoveNotation(const Move& move) const {
    // Simple algebraic notation
    std::string notation = "";
    auto colToChar = [](int c) { return (char)('a' + c); };
    auto rowToChar = [](int r) { return (char)('8' - r); };

    // This doesn't account for piece type, but requirements asked for PGN or algebraic.
    // Let's do a simple one: e2e4
    notation += colToChar(move.from.col);
    notation += rowToChar(move.from.row);
    notation += colToChar(move.to.col);
    notation += rowToChar(move.to.row);
    
    if (move.type == MoveType::Promotion) {
        if (move.promotionType == PieceType::Queen) notation += "q";
        if (move.promotionType == PieceType::Rook) notation += "r";
        if (move.promotionType == PieceType::Bishop) notation += "b";
        if (move.promotionType == PieceType::Knight) notation += "n";
    }

    return notation;
}

} // namespace chess
