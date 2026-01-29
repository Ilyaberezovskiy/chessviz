#include "rules.h"
#include <cmath>
#include <algorithm>

namespace chess {

std::vector<Move> Rules::getValidMoves(const Board& board, Position pos, const std::vector<Move>& history) {
    std::vector<Move> pseudoMoves = getPseudoLegalMoves(board, pos, history);
    std::vector<Move> validMoves;

    Color color = board.getPiece(pos).getColor();

    for (const auto& move : pseudoMoves) {
        // Simulate move
        Board tempBoard = board;
        Piece movedPiece = tempBoard.getPiece(move.from);
        tempBoard.clearSquare(move.from);
        
        if (move.type == MoveType::Promotion) {
            movedPiece = Piece(move.promotionType, color, move.to);
        }
        
        tempBoard.setPiece(move.to, movedPiece);
        
        if (move.type == MoveType::EnPassant) {
            int captureRow = (color == Color::White) ? move.to.row + 1 : move.to.row - 1;
            tempBoard.clearSquare({captureRow, move.to.col});
        }

        if (!isKingInCheck(tempBoard, color, history)) {
            validMoves.push_back(move);
        }
    }

    // Add castling
    if (board.getPiece(pos).getType() == PieceType::King) {
        if (canCastle(board, color, true, history)) {
            validMoves.push_back(Move(pos, {pos.row, 6}, MoveType::Castling));
        }
        if (canCastle(board, color, false, history)) {
            validMoves.push_back(Move(pos, {pos.row, 2}, MoveType::Castling));
        }
    }

    return validMoves;
}

std::vector<Move> Rules::getPseudoLegalMoves(const Board& board, Position pos, const std::vector<Move>& history) {
    std::vector<Move> moves;
    const Piece& piece = board.getPiece(pos);
    if (piece.isEmpty()) return moves;

    Color color = piece.getColor();
    int r = pos.row;
    int c = pos.col;

    auto addMove = [&](int tr, int tc) {
        if (board.isValidPosition({tr, tc})) {
            const Piece& target = board.getPiece({tr, tc});
            if (target.isEmpty() || target.getColor() != color) {
                moves.push_back(Move(pos, {tr, tc}));
                return true; // occupied but different color
            }
        }
        return false;
    };

    auto addSlidingMoves = [&](const std::vector<std::pair<int, int>>& dirs) {
        for (auto [dr, dc] : dirs) {
            for (int i = 1; i < 8; ++i) {
                int tr = r + dr * i;
                int tc = c + dc * i;
                if (!board.isValidPosition({tr, tc})) break;
                const Piece& target = board.getPiece({tr, tc});
                if (target.isEmpty()) {
                    moves.push_back(Move(pos, {tr, tc}));
                } else {
                    if (target.getColor() != color) {
                        moves.push_back(Move(pos, {tr, tc}));
                    }
                    break;
                }
            }
        }
    };

    switch (piece.getType()) {
        case PieceType::Pawn: {
            int dir = (color == Color::White) ? -1 : 1;
            int startRow = (color == Color::White) ? 6 : 1;
            
            // Forward
            if (board.isValidPosition({r + dir, c}) && !board.isSquareOccupied({r + dir, c})) {
                if (r + dir == 0 || r + dir == 7) {
                    moves.push_back(Move(pos, {r + dir, c}, MoveType::Promotion, PieceType::Queen));
                    moves.push_back(Move(pos, {r + dir, c}, MoveType::Promotion, PieceType::Rook));
                    moves.push_back(Move(pos, {r + dir, c}, MoveType::Promotion, PieceType::Bishop));
                    moves.push_back(Move(pos, {r + dir, c}, MoveType::Promotion, PieceType::Knight));
                } else {
                    moves.push_back(Move(pos, {r + dir, c}));
                    if (r == startRow && !board.isSquareOccupied({r + 2 * dir, c})) {
                        moves.push_back(Move(pos, {r + 2 * dir, c}));
                    }
                }
            }
            
            // Captures
            for (int dc : {-1, 1}) {
                int tr = r + dir;
                int tc = c + dc;
                if (board.isValidPosition({tr, tc})) {
                    if (board.isSquareOccupied({tr, tc}) && board.getPiece({tr, tc}).getColor() != color) {
                        if (tr == 0 || tr == 7) {
                            moves.push_back(Move(pos, {tr, tc}, MoveType::Promotion, PieceType::Queen));
                            moves.push_back(Move(pos, {tr, tc}, MoveType::Promotion, PieceType::Rook));
                            moves.push_back(Move(pos, {tr, tc}, MoveType::Promotion, PieceType::Bishop));
                            moves.push_back(Move(pos, {tr, tc}, MoveType::Promotion, PieceType::Knight));
                        } else {
                            moves.push_back(Move(pos, {tr, tc}));
                        }
                    } else if (canEnPassant(board, pos, {tr, tc}, history)) {
                        moves.push_back(Move(pos, {tr, tc}, MoveType::EnPassant));
                    }
                }
            }
            break;
        }
        case PieceType::Knight:
            for (int dr : {-2, -1, 1, 2}) {
                for (int dc : {-2, -1, 1, 2}) {
                    if (std::abs(dr) + std::abs(dc) == 3) {
                        addMove(r + dr, c + dc);
                    }
                }
            }
            break;
        case PieceType::Bishop:
            addSlidingMoves({{-1, -1}, {-1, 1}, {1, -1}, {1, 1}});
            break;
        case PieceType::Rook:
            addSlidingMoves({{-1, 0}, {1, 0}, {0, -1}, {0, 1}});
            break;
        case PieceType::Queen:
            addSlidingMoves({{-1, -1}, {-1, 1}, {1, -1}, {1, 1}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}});
            break;
        case PieceType::King:
            for (int dr : {-1, 0, 1}) {
                for (int dc : {-1, 0, 1}) {
                    if (dr != 0 || dc != 0) {
                        addMove(r + dr, c + dc);
                    }
                }
            }
            break;
        default: break;
    }

    return moves;
}

bool Rules::isKingInCheck(const Board& board, Color color, const std::vector<Move>& history) {
    Position kingPos = {-1, -1};
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Piece& p = board.getPiece({r, c});
            if (p.getType() == PieceType::King && p.getColor() == color) {
                kingPos = {r, c};
                break;
            }
        }
    }
    if (kingPos.row == -1) return false; // Should not happen

    return isSquareAttacked(board, kingPos, color == Color::White ? Color::Black : Color::White);
}

bool Rules::isSquareAttacked(const Board& board, Position pos, Color attackerColor) {
    // Check if any piece of attackerColor can move to pos
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Piece& p = board.getPiece({r, c});
            if (p.getColor() == attackerColor) {
                // Pawns are special because their attack is different from their move
                if (p.getType() == PieceType::Pawn) {
                    int dir = (attackerColor == Color::White) ? -1 : 1;
                    if (pos.row == r + dir && (pos.col == c - 1 || pos.col == c + 1)) {
                        return true;
                    }
                } else {
                    // For other pieces, pseudo-legal moves include captures
                    // We don't need to check for checkmate recursion here because we only care about attackers
                    std::vector<Move> moves = getPseudoLegalMoves(board, {r, c}, {});
                    for (const auto& m : moves) {
                        if (m.to == pos) return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Rules::isCheckmate(const Board& board, Color color, const std::vector<Move>& history) {
    if (!isKingInCheck(board, color, history)) return false;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board.getPiece({r, c}).getColor() == color) {
                if (!getValidMoves(board, {r, c}, history).empty()) return false;
            }
        }
    }
    return true;
}

bool Rules::isStalemate(const Board& board, Color color, const std::vector<Move>& history) {
    if (isKingInCheck(board, color, history)) return false;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board.getPiece({r, c}).getColor() == color) {
                if (!getValidMoves(board, {r, c}, history).empty()) return false;
            }
        }
    }
    return true;
}

bool Rules::canCastle(const Board& board, Color color, bool kingSide, const std::vector<Move>& history) {
    if (isKingInCheck(board, color, history)) return false;

    int row = (color == Color::White) ? 7 : 0;
    Position kingPos = {row, 4};
    Position rookPos = kingSide ? Position{row, 7} : Position{row, 0};

    const Piece& king = board.getPiece(kingPos);
    const Piece& rook = board.getPiece(rookPos);

    if (king.getType() != PieceType::King || king.getColor() != color) return false;
    if (rook.getType() != PieceType::Rook || rook.getColor() != color) return false;

    // Check history
    for (const auto& move : history) {
        if (move.from == kingPos || move.from == rookPos) return false;
    }

    // Check squares between
    int start = kingSide ? 5 : 1;
    int end = kingSide ? 6 : 3;
    for (int c = start; c <= end; ++c) {
        if (board.isSquareOccupied({row, c})) return false;
    }

    // Check if king passes through check
    int step = kingSide ? 1 : -1;
    if (isSquareAttacked(board, {row, 4 + step}, color == Color::White ? Color::Black : Color::White)) return false;
    if (isSquareAttacked(board, {row, 4 + 2 * step}, color == Color::White ? Color::Black : Color::White)) return false;

    return true;
}

bool Rules::canEnPassant(const Board& board, Position from, Position to, const std::vector<Move>& history) {
    if (history.empty()) return false;
    const Move& lastMove = history.back();
    const Piece& pawn = board.getPiece(from);

    if (pawn.getType() != PieceType::Pawn) return false;
    
    int dir = (pawn.getColor() == Color::White) ? -1 : 1;
    if (to.row != (pawn.getColor() == Color::White ? 2 : 5)) return false;
    if (std::abs(to.col - from.col) != 1) return false;

    // Last move must be a pawn double step
    if (lastMove.to.col == to.col && std::abs(lastMove.from.row - lastMove.to.row) == 2) {
        const Piece& lastPiece = board.getPiece(lastMove.to);
        if (lastPiece.getType() == PieceType::Pawn && lastPiece.getColor() != pawn.getColor()) {
            return true;
        }
    }

    return false;
}

} // namespace chess
