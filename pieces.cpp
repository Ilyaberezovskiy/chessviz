#include "pieces.h"
#include "board.h"

char Piece::fenChar() const {
    char c = '?';
    switch (type) {
        case PieceType::Pawn:   c = 'p'; break;
        case PieceType::Rook:   c = 'r'; break;
        case PieceType::Knight: c = 'n'; break;
        case PieceType::Bishop: c = 'b'; break;
        case PieceType::Queen:  c = 'q'; break;
        case PieceType::King:   c = 'k'; break;
    }
    if (color == Color::White) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return c;
}

// Скользящие ходы (ладья, слон, ферзь)
std::vector<Move> generateSlidingMoves(const Square& pos, const Board& board,
                                        Color color,
                                        const std::vector<std::pair<int,int>>& directions) {
    std::vector<Move> moves;
    for (auto [dr, dc] : directions) {
        for (int i = 1; i < 8; ++i) {
            Square target{pos.row + dr * i, pos.col + dc * i};
            if (!target.isValid()) break;

            const auto* piece = board.getPiece(target);
            if (piece == nullptr) {
                moves.push_back({pos, target});
            } else {
                if (piece->color != color) {
                    moves.push_back({pos, target}); // взятие
                }
                break; // дальше не идём
            }
        }
    }
    return moves;
}

// ===== Пешка =====
std::string Pawn::getSymbol() const {
    return color == Color::White ? "♟" : "♙";
}

std::vector<Move> Pawn::generatePseudoLegalMoves(const Square& pos, const Board& board) const {
    std::vector<Move> moves;
    int direction = (color == Color::White) ? 1 : -1;
    int startRow = (color == Color::White) ? 1 : 6;
    int promotionRow = (color == Color::White) ? 7 : 0;

    auto addMoveOrPromotion = [&](Square from, Square to) {
        if (to.row == promotionRow) {
            // Превращение — генерируем 4 варианта
            for (char p : {'q', 'r', 'b', 'n'}) {
                moves.push_back({from, to, p});
            }
        } else {
            moves.push_back({from, to});
        }
    };

    // Ход на одну клетку вперёд
    Square oneStep{pos.row + direction, pos.col};
    if (oneStep.isValid() && board.getPiece(oneStep) == nullptr) {
        addMoveOrPromotion(pos, oneStep);

        // Двойной ход с начальной позиции
        if (pos.row == startRow) {
            Square twoStep{pos.row + 2 * direction, pos.col};
            if (board.getPiece(twoStep) == nullptr) {
                moves.push_back({pos, twoStep});
            }
        }
    }

    // Взятие по диагонали
    for (int dc : {-1, 1}) {
        Square diag{pos.row + direction, pos.col + dc};
        if (!diag.isValid()) continue;

        const auto* target = board.getPiece(diag);
        if (target != nullptr && target->color != color) {
            addMoveOrPromotion(pos, diag);
        }

        // En passant
        if (board.getEnPassantTarget().has_value() && board.getEnPassantTarget().value() == diag) {
            moves.push_back({pos, diag});
        }
    }

    return moves;
}

std::unique_ptr<Piece> Pawn::clone() const {
    auto p = std::make_unique<Pawn>(color);
    p->moved_ = moved_;
    return p;
}

// ===== Ладья =====
std::string Rook::getSymbol() const {
    return color == Color::White ? "♜" : "♖";
}

std::vector<Move> Rook::generatePseudoLegalMoves(const Square& pos, const Board& board) const {
    return generateSlidingMoves(pos, board, color, {{1,0},{-1,0},{0,1},{0,-1}});
}

std::unique_ptr<Piece> Rook::clone() const {
    auto p = std::make_unique<Rook>(color);
    p->moved_ = moved_;
    return p;
}

// ===== Конь =====
std::string Knight::getSymbol() const {
    return color == Color::White ? "♞" : "♘";
}

std::vector<Move> Knight::generatePseudoLegalMoves(const Square& pos, const Board& board) const {
    std::vector<Move> moves;
    static const std::vector<std::pair<int,int>> offsets = {
        {2,1},{2,-1},{-2,1},{-2,-1},{1,2},{1,-2},{-1,2},{-1,-2}
    };
    for (auto [dr, dc] : offsets) {
        Square target{pos.row + dr, pos.col + dc};
        if (!target.isValid()) continue;
        const auto* piece = board.getPiece(target);
        if (piece == nullptr || piece->color != color) {
            moves.push_back({pos, target});
        }
    }
    return moves;
}

std::unique_ptr<Piece> Knight::clone() const {
    auto p = std::make_unique<Knight>(color);
    p->moved_ = moved_;
    return p;
}

// ===== Слон =====
std::string Bishop::getSymbol() const {
    return color == Color::White ? "♝" : "♗";
}

std::vector<Move> Bishop::generatePseudoLegalMoves(const Square& pos, const Board& board) const {
    return generateSlidingMoves(pos, board, color, {{1,1},{1,-1},{-1,1},{-1,-1}});
}

std::unique_ptr<Piece> Bishop::clone() const {
    auto p = std::make_unique<Bishop>(color);
    p->moved_ = moved_;
    return p;
}

// ===== Ферзь =====
std::string Queen::getSymbol() const {
    return color == Color::White ? "♛" : "♕";
}

std::vector<Move> Queen::generatePseudoLegalMoves(const Square& pos, const Board& board) const {
    return generateSlidingMoves(pos, board, color,
        {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}});
}

std::unique_ptr<Piece> Queen::clone() const {
    auto p = std::make_unique<Queen>(color);
    p->moved_ = moved_;
    return p;
}

// ===== Король =====
std::string King::getSymbol() const {
    return color == Color::White ? "♚" : "♔";
}

std::vector<Move> King::generatePseudoLegalMoves(const Square& pos, const Board& board) const {
    std::vector<Move> moves;

    // 8 соседних клеток
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            Square target{pos.row + dr, pos.col + dc};
            if (!target.isValid()) continue;
            const auto* piece = board.getPiece(target);
            if (piece == nullptr || piece->color != color) {
                moves.push_back({pos, target});
            }
        }
    }

    // Рокировка (базовые условия — полная проверка в Board)
    if (!moved_) {
        int row = (color == Color::White) ? 0 : 7;
        if (pos.row == row && pos.col == 4) {
            // Короткая рокировка (O-O)
            if (board.canCastleKingside(color)) {
                const auto* rook = board.getPiece({row, 7});
                if (rook && rook->type == PieceType::Rook && !rook->moved_) {
                    // Проверяем, что поля между королём и ладьёй свободны
                    if (board.getPiece({row, 5}) == nullptr &&
                        board.getPiece({row, 6}) == nullptr) {
                        moves.push_back({pos, {row, 6}});
                    }
                }
            }
            // Длинная рокировка (O-O-O)
            if (board.canCastleQueenside(color)) {
                const auto* rook = board.getPiece({row, 0});
                if (rook && rook->type == PieceType::Rook && !rook->moved_) {
                    if (board.getPiece({row, 1}) == nullptr &&
                        board.getPiece({row, 2}) == nullptr &&
                        board.getPiece({row, 3}) == nullptr) {
                        moves.push_back({pos, {row, 2}});
                    }
                }
            }
        }
    }

    return moves;
}

std::unique_ptr<Piece> King::clone() const {
    auto p = std::make_unique<King>(color);
    p->moved_ = moved_;
    return p;
}
