#include "GameState.h"
#include "MoveValidator.h"
#include <sstream>
#include <map>

GameState::GameState() {
    reset();
}

void GameState::reset() {
    board.reset();
    currentPlayer = Color::WHITE;
    status = GameStatus::PLAYING;
    history.clear();
    lastMove = Move();
    halfMoveClock = 0;
    fullMoveNumber = 1;
    
    whiteKingMoved = false;
    whiteRookKingsideMoved = false;
    whiteRookQueensideMoved = false;
    blackKingMoved = false;
    blackRookKingsideMoved = false;
    blackRookQueensideMoved = false;
}

bool GameState::makeMove(const Move& move) {
    if (status == GameStatus::CHECKMATE || status == GameStatus::STALEMATE || status == GameStatus::DRAW) {
        return false;
    }
    
    if (!MoveValidator::isValidMove(board, move, currentPlayer)) {
        return false;
    }
    
    HistoryEntry entry;
    entry.move = move;
    entry.capturedPiece = board.getPiece(move.to);
    entry.enPassantTarget = board.getEnPassantTarget();
    
    Piece& piece = board.getPieceRef(move.from);
    
    if (piece.getType() == PieceType::KING) {
        if (currentPlayer == Color::WHITE) {
            entry.kingMoved = whiteKingMoved;
            whiteKingMoved = true;
        } else {
            entry.kingMoved = blackKingMoved;
            blackKingMoved = true;
        }
        
        if (abs(move.to.col - move.from.col) == 2) {
            int rookFromCol = (move.to.col > move.from.col) ? 7 : 0;
            int rookToCol = (move.to.col > move.from.col) ? 5 : 3;
            
            Piece& rook = board.getPieceRef(Position(move.from.row, rookFromCol));
            board.setPiece(Position(move.from.row, rookToCol), rook);
            board.removePiece(Position(move.from.row, rookFromCol));
            board.getPieceRef(Position(move.from.row, rookToCol)).setHasMoved(true);
        }
    }
    
    if (piece.getType() == PieceType::ROOK) {
        if (currentPlayer == Color::WHITE) {
            if (move.from.col == 7) {
                entry.rookMoved = whiteRookKingsideMoved;
                whiteRookKingsideMoved = true;
            } else if (move.from.col == 0) {
                entry.rookMoved = whiteRookQueensideMoved;
                whiteRookQueensideMoved = true;
            }
        } else {
            if (move.from.col == 7) {
                entry.rookMoved = blackRookKingsideMoved;
                blackRookKingsideMoved = true;
            } else if (move.from.col == 0) {
                entry.rookMoved = blackRookQueensideMoved;
                blackRookQueensideMoved = true;
            }
        }
    }
    
    history.push_back(entry);
    
    board.setEnPassantTarget(Position());
    
    if (piece.getType() == PieceType::PAWN) {
        halfMoveClock = 0;
        
        if (abs(move.to.row - move.from.row) == 2) {
            int enPassantRow = (move.from.row + move.to.row) / 2;
            board.setEnPassantTarget(Position(enPassantRow, move.from.col));
        }
        
        Position enPassant = entry.enPassantTarget;
        if (enPassant.isValid() && move.to == enPassant) {
            int direction = (currentPlayer == Color::WHITE) ? 1 : -1;
            board.removePiece(Position(move.to.row + direction, move.to.col));
        }
        
        if ((currentPlayer == Color::WHITE && move.to.row == 0) ||
            (currentPlayer == Color::BLACK && move.to.row == 7)) {
            if (move.promotionType == PieceType::NONE) {
                piece = Piece(PieceType::QUEEN, currentPlayer);
            } else {
                piece = Piece(move.promotionType, currentPlayer);
            }
        }
    } else if (!entry.capturedPiece.isEmpty()) {
        halfMoveClock = 0;
    } else {
        halfMoveClock++;
    }
    
    piece.setHasMoved(true);
    board.setPiece(move.to, piece);
    board.removePiece(move.from);
    
    lastMove = move;
    
    switchPlayer();
    updateGameStatus();
    
    if (currentPlayer == Color::WHITE) {
        fullMoveNumber++;
    }
    
    return true;
}

bool GameState::undoMove() {
    if (history.empty()) {
        return false;
    }
    
    HistoryEntry entry = history.back();
    history.pop_back();
    
    switchPlayer();
    
    Piece piece = board.getPiece(entry.move.to);
    
    if (piece.getType() == PieceType::PAWN) {
        if ((currentPlayer == Color::WHITE && entry.move.to.row == 0) ||
            (currentPlayer == Color::BLACK && entry.move.to.row == 7)) {
            piece = Piece(PieceType::PAWN, currentPlayer);
        }
        
        if (entry.move.isEnPassant) {
            int direction = (currentPlayer == Color::WHITE) ? 1 : -1;
            Position capturedPawnPos(entry.move.to.row + direction, entry.move.to.col);
            board.setPiece(capturedPawnPos, Piece(PieceType::PAWN,
                (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE));
        }
    }
    
    if (piece.getType() == PieceType::KING && abs(entry.move.to.col - entry.move.from.col) == 2) {
        int rookToCol = (entry.move.to.col > entry.move.from.col) ? 5 : 3;
        int rookFromCol = (entry.move.to.col > entry.move.from.col) ? 7 : 0;
        
        Piece rook = board.getPiece(Position(entry.move.from.row, rookToCol));
        board.setPiece(Position(entry.move.from.row, rookFromCol), rook);
        board.removePiece(Position(entry.move.from.row, rookToCol));
        
        if (currentPlayer == Color::WHITE) {
            whiteKingMoved = entry.kingMoved;
            if (rookFromCol == 7) {
                whiteRookKingsideMoved = entry.rookMoved;
            } else {
                whiteRookQueensideMoved = entry.rookMoved;
            }
        } else {
            blackKingMoved = entry.kingMoved;
            if (rookFromCol == 7) {
                blackRookKingsideMoved = entry.rookMoved;
            } else {
                blackRookQueensideMoved = entry.rookMoved;
            }
        }
    }
    
    board.setPiece(entry.move.from, piece);
    board.setPiece(entry.move.to, entry.capturedPiece);
    board.setEnPassantTarget(entry.enPassantTarget);
    
    if (history.empty()) {
        lastMove = Move();
    } else {
        lastMove = history.back().move;
    }
    
    updateGameStatus();
    
    if (currentPlayer == Color::BLACK) {
        fullMoveNumber--;
    }
    
    return true;
}

std::vector<Position> GameState::getLegalMovesForPiece(const Position& pos) {
    return MoveValidator::getLegalMoves(board, pos, currentPlayer);
}

bool GameState::canCastleKingside(Color color) const {
    if (color == Color::WHITE) {
        return !whiteKingMoved && !whiteRookKingsideMoved;
    } else {
        return !blackKingMoved && !blackRookKingsideMoved;
    }
}

bool GameState::canCastleQueenside(Color color) const {
    if (color == Color::WHITE) {
        return !whiteKingMoved && !whiteRookQueensideMoved;
    } else {
        return !blackKingMoved && !blackRookQueensideMoved;
    }
}

void GameState::updateGameStatus() {
    bool inCheck = MoveValidator::isKingInCheck(board, currentPlayer);
    bool hasLegalMoves = MoveValidator::hasLegalMoves(board, currentPlayer);
    
    if (!hasLegalMoves) {
        if (inCheck) {
            status = GameStatus::CHECKMATE;
        } else {
            status = GameStatus::STALEMATE;
        }
    } else if (inCheck) {
        status = GameStatus::CHECK;
    } else if (halfMoveClock >= 100) {
        status = GameStatus::DRAW;
    } else if (isThreefoldRepetition()) {
        status = GameStatus::DRAW;
    } else {
        status = GameStatus::PLAYING;
    }
}

void GameState::switchPlayer() {
    currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
}

bool GameState::isThreefoldRepetition() const {
    if (history.size() < 8) return false;
    
    std::string currentHash = getBoardHash();
    int count = 0;
    
    for (size_t i = 0; i < history.size(); i++) {
        if (getBoardHash() == currentHash) {
            count++;
            if (count >= 3) return true;
        }
    }
    
    return false;
}

std::string GameState::getBoardHash() const {
    std::stringstream ss;
    
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            const Piece& piece = board.getPiece(Position(row, col));
            ss << static_cast<int>(piece.getType()) << static_cast<int>(piece.getColor());
        }
    }
    
    ss << static_cast<int>(currentPlayer);
    ss << board.getEnPassantTarget().row << board.getEnPassantTarget().col;
    
    return ss.str();
}
