#include "ChessBoard.h"

ChessBoard::ChessBoard() {
    reset();
}

void ChessBoard::reset() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            board[row][col] = Piece();
        }
    }
    
    board[0][0] = Piece(PieceType::ROOK, Color::BLACK);
    board[0][1] = Piece(PieceType::KNIGHT, Color::BLACK);
    board[0][2] = Piece(PieceType::BISHOP, Color::BLACK);
    board[0][3] = Piece(PieceType::QUEEN, Color::BLACK);
    board[0][4] = Piece(PieceType::KING, Color::BLACK);
    board[0][5] = Piece(PieceType::BISHOP, Color::BLACK);
    board[0][6] = Piece(PieceType::KNIGHT, Color::BLACK);
    board[0][7] = Piece(PieceType::ROOK, Color::BLACK);
    
    for (int col = 0; col < 8; col++) {
        board[1][col] = Piece(PieceType::PAWN, Color::BLACK);
    }
    
    board[7][0] = Piece(PieceType::ROOK, Color::WHITE);
    board[7][1] = Piece(PieceType::KNIGHT, Color::WHITE);
    board[7][2] = Piece(PieceType::BISHOP, Color::WHITE);
    board[7][3] = Piece(PieceType::QUEEN, Color::WHITE);
    board[7][4] = Piece(PieceType::KING, Color::WHITE);
    board[7][5] = Piece(PieceType::BISHOP, Color::WHITE);
    board[7][6] = Piece(PieceType::KNIGHT, Color::WHITE);
    board[7][7] = Piece(PieceType::ROOK, Color::WHITE);
    
    for (int col = 0; col < 8; col++) {
        board[6][col] = Piece(PieceType::PAWN, Color::WHITE);
    }
    
    enPassantTarget = Position();
}

const Piece& ChessBoard::getPiece(const Position& pos) const {
    if (!pos.isValid()) {
        static Piece emptyPiece;
        return emptyPiece;
    }
    return board[pos.row][pos.col];
}

Piece& ChessBoard::getPieceRef(const Position& pos) {
    return board[pos.row][pos.col];
}

void ChessBoard::setPiece(const Position& pos, const Piece& piece) {
    if (pos.isValid()) {
        board[pos.row][pos.col] = piece;
    }
}

void ChessBoard::removePiece(const Position& pos) {
    if (pos.isValid()) {
        board[pos.row][pos.col] = Piece();
    }
}

bool ChessBoard::isOccupied(const Position& pos) const {
    if (!pos.isValid()) return false;
    return !board[pos.row][pos.col].isEmpty();
}

bool ChessBoard::isOccupiedByColor(const Position& pos, Color color) const {
    if (!pos.isValid()) return false;
    return isOccupied(pos) && board[pos.row][pos.col].getColor() == color;
}

Position ChessBoard::findKing(Color color) const {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            const Piece& piece = board[row][col];
            if (piece.getType() == PieceType::KING && piece.getColor() == color) {
                return Position(row, col);
            }
        }
    }
    return Position();
}

std::vector<Position> ChessBoard::getAllPiecesOfColor(Color color) const {
    std::vector<Position> pieces;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col].getColor() == color) {
                pieces.push_back(Position(row, col));
            }
        }
    }
    return pieces;
}
