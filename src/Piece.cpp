#include "Piece.h"
#include "ChessBoard.h"

Piece::Piece() : type(PieceType::NONE), color(Color::NONE), hasMoved(false) {}

Piece::Piece(PieceType t, Color c) : type(t), color(c), hasMoved(false) {}

std::vector<Position> Piece::getPossibleMoves(const Position& pos, const ChessBoard& board) const {
    switch (type) {
        case PieceType::PAWN:
            return getPawnMoves(pos, board);
        case PieceType::KNIGHT:
            return getKnightMoves(pos, board);
        case PieceType::BISHOP:
            return getBishopMoves(pos, board);
        case PieceType::ROOK:
            return getRookMoves(pos, board);
        case PieceType::QUEEN:
            return getQueenMoves(pos, board);
        case PieceType::KING:
            return getKingMoves(pos, board);
        default:
            return {};
    }
}

std::vector<Position> Piece::getPawnMoves(const Position& pos, const ChessBoard& board) const {
    std::vector<Position> moves;
    int direction = (color == Color::WHITE) ? -1 : 1;
    
    Position forward(pos.row + direction, pos.col);
    if (forward.isValid() && !board.isOccupied(forward)) {
        moves.push_back(forward);
        
        int startRow = (color == Color::WHITE) ? 6 : 1;
        if (pos.row == startRow) {
            Position doubleForward(pos.row + 2 * direction, pos.col);
            if (doubleForward.isValid() && !board.isOccupied(doubleForward)) {
                moves.push_back(doubleForward);
            }
        }
    }
    
    Position captureLeft(pos.row + direction, pos.col - 1);
    if (captureLeft.isValid() && board.isOccupiedByColor(captureLeft, 
        (color == Color::WHITE) ? Color::BLACK : Color::WHITE)) {
        moves.push_back(captureLeft);
    }
    
    Position captureRight(pos.row + direction, pos.col + 1);
    if (captureRight.isValid() && board.isOccupiedByColor(captureRight,
        (color == Color::WHITE) ? Color::BLACK : Color::WHITE)) {
        moves.push_back(captureRight);
    }
    
    Position enPassant = board.getEnPassantTarget();
    if (enPassant.isValid()) {
        if (captureLeft == enPassant) {
            moves.push_back(captureLeft);
        }
        if (captureRight == enPassant) {
            moves.push_back(captureRight);
        }
    }
    
    return moves;
}

std::vector<Position> Piece::getKnightMoves(const Position& pos, const ChessBoard& board) const {
    std::vector<Position> moves;
    const int knightMoves[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };
    
    for (const auto& move : knightMoves) {
        Position newPos(pos.row + move[0], pos.col + move[1]);
        if (newPos.isValid() && !board.isOccupiedByColor(newPos, color)) {
            moves.push_back(newPos);
        }
    }
    
    return moves;
}

std::vector<Position> Piece::getBishopMoves(const Position& pos, const ChessBoard& board) const {
    std::vector<Position> moves;
    addDirectionalMoves(moves, pos, -1, -1, board);
    addDirectionalMoves(moves, pos, -1, 1, board);
    addDirectionalMoves(moves, pos, 1, -1, board);
    addDirectionalMoves(moves, pos, 1, 1, board);
    return moves;
}

std::vector<Position> Piece::getRookMoves(const Position& pos, const ChessBoard& board) const {
    std::vector<Position> moves;
    addDirectionalMoves(moves, pos, -1, 0, board);
    addDirectionalMoves(moves, pos, 1, 0, board);
    addDirectionalMoves(moves, pos, 0, -1, board);
    addDirectionalMoves(moves, pos, 0, 1, board);
    return moves;
}

std::vector<Position> Piece::getQueenMoves(const Position& pos, const ChessBoard& board) const {
    std::vector<Position> moves;
    addDirectionalMoves(moves, pos, -1, -1, board);
    addDirectionalMoves(moves, pos, -1, 0, board);
    addDirectionalMoves(moves, pos, -1, 1, board);
    addDirectionalMoves(moves, pos, 0, -1, board);
    addDirectionalMoves(moves, pos, 0, 1, board);
    addDirectionalMoves(moves, pos, 1, -1, board);
    addDirectionalMoves(moves, pos, 1, 0, board);
    addDirectionalMoves(moves, pos, 1, 1, board);
    return moves;
}

std::vector<Position> Piece::getKingMoves(const Position& pos, const ChessBoard& board) const {
    std::vector<Position> moves;
    for (int rowDir = -1; rowDir <= 1; rowDir++) {
        for (int colDir = -1; colDir <= 1; colDir++) {
            if (rowDir == 0 && colDir == 0) continue;
            addDirectionalMoves(moves, pos, rowDir, colDir, board, false);
        }
    }
    return moves;
}

void Piece::addDirectionalMoves(std::vector<Position>& moves, const Position& pos,
                               int rowDir, int colDir, const ChessBoard& board,
                               bool unlimited) const {
    Position current(pos.row + rowDir, pos.col + colDir);
    
    while (current.isValid()) {
        if (board.isOccupied(current)) {
            if (!board.isOccupiedByColor(current, color)) {
                moves.push_back(current);
            }
            break;
        }
        
        moves.push_back(current);
        
        if (!unlimited) break;
        
        current.row += rowDir;
        current.col += colDir;
    }
}
