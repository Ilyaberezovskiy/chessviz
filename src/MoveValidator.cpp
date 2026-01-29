#include "MoveValidator.h"

bool MoveValidator::isValidMove(const ChessBoard& board, const Move& move, Color currentPlayer) {
    if (!move.from.isValid() || !move.to.isValid()) return false;
    if (move.from == move.to) return false;
    
    const Piece& piece = board.getPiece(move.from);
    if (piece.isEmpty() || piece.getColor() != currentPlayer) return false;
    
    if (board.isOccupiedByColor(move.to, currentPlayer)) return false;
    
    std::vector<Position> possibleMoves = piece.getPossibleMoves(move.from, board);
    bool moveFound = false;
    for (const Position& pos : possibleMoves) {
        if (pos == move.to) {
            moveFound = true;
            break;
        }
    }
    if (!moveFound) return false;
    
    if (wouldMoveCauseCheck(board, move, currentPlayer)) return false;
    
    return true;
}

bool MoveValidator::isSquareUnderAttack(const ChessBoard& board, const Position& pos, Color attackingColor) {
    std::vector<Position> attackers = board.getAllPiecesOfColor(attackingColor);
    
    for (const Position& attackerPos : attackers) {
        const Piece& attacker = board.getPiece(attackerPos);
        std::vector<Position> moves = attacker.getPossibleMoves(attackerPos, board);
        
        for (const Position& move : moves) {
            if (move == pos) return true;
        }
    }
    
    return false;
}

bool MoveValidator::isKingInCheck(const ChessBoard& board, Color kingColor) {
    Position kingPos = board.findKing(kingColor);
    if (!kingPos.isValid()) return false;
    
    Color opponentColor = (kingColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return isSquareUnderAttack(board, kingPos, opponentColor);
}

bool MoveValidator::wouldMoveCauseCheck(ChessBoard board, const Move& move, Color currentPlayer) {
    Piece piece = board.getPiece(move.from);
    board.setPiece(move.to, piece);
    board.removePiece(move.from);
    
    if (move.isEnPassant) {
        int direction = (currentPlayer == Color::WHITE) ? 1 : -1;
        Position capturedPawnPos(move.to.row + direction, move.to.col);
        board.removePiece(capturedPawnPos);
    }
    
    return isKingInCheck(board, currentPlayer);
}

std::vector<Position> MoveValidator::getLegalMoves(const ChessBoard& board, const Position& pos, Color currentPlayer) {
    std::vector<Position> legalMoves;
    
    const Piece& piece = board.getPiece(pos);
    if (piece.isEmpty() || piece.getColor() != currentPlayer) {
        return legalMoves;
    }
    
    std::vector<Position> possibleMoves = piece.getPossibleMoves(pos, board);
    
    for (const Position& target : possibleMoves) {
        Move move(pos, target);
        
        if (piece.getType() == PieceType::PAWN) {
            Position enPassant = board.getEnPassantTarget();
            if (enPassant.isValid() && target == enPassant) {
                move.isEnPassant = true;
            }
        }
        
        if (!wouldMoveCauseCheck(board, move, currentPlayer)) {
            legalMoves.push_back(target);
        }
    }
    
    if (piece.getType() == PieceType::KING && !piece.getHasMoved()) {
        Position kingsideRookPos(pos.row, 7);
        if (canCastle(board, pos, kingsideRookPos, currentPlayer)) {
            legalMoves.push_back(Position(pos.row, 6));
        }
        
        Position queensideRookPos(pos.row, 0);
        if (canCastle(board, pos, queensideRookPos, currentPlayer)) {
            legalMoves.push_back(Position(pos.row, 2));
        }
    }
    
    return legalMoves;
}

bool MoveValidator::canCastle(const ChessBoard& board, const Position& kingPos,
                              const Position& rookPos, Color currentPlayer) {
    const Piece& king = board.getPiece(kingPos);
    const Piece& rook = board.getPiece(rookPos);
    
    if (king.getHasMoved() || rook.isEmpty() || rook.getHasMoved()) {
        return false;
    }
    
    if (isKingInCheck(board, currentPlayer)) {
        return false;
    }
    
    int direction = (rookPos.col > kingPos.col) ? 1 : -1;
    int endCol = (direction == 1) ? 6 : 2;
    
    for (int col = kingPos.col + direction; col != rookPos.col; col += direction) {
        Position pos(kingPos.row, col);
        if (board.isOccupied(pos)) {
            return false;
        }
    }
    
    Color opponentColor = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
    for (int col = kingPos.col; col != endCol + direction; col += direction) {
        Position pos(kingPos.row, col);
        if (isSquareUnderAttack(board, pos, opponentColor)) {
            return false;
        }
    }
    
    return true;
}

bool MoveValidator::hasLegalMoves(const ChessBoard& board, Color color) {
    std::vector<Position> pieces = board.getAllPiecesOfColor(color);
    
    for (const Position& pos : pieces) {
        std::vector<Position> moves = getLegalMoves(board, pos, color);
        if (!moves.empty()) {
            return true;
        }
    }
    
    return false;
}
