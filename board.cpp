#include "board.h"
#include <iostream>
#include <sstream>
#include <algorithm>

Board::Board() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            grid_[r][c] = nullptr;
}

void Board::placePiece(int row, int col, std::unique_ptr<Piece> piece) {
    grid_[row][col] = std::move(piece);
}

void Board::setupInitialPosition() {
    // Белые фигуры (ряд 0 = rank 1)
    placePiece(0, 0, std::make_unique<Rook>(Color::White));
    placePiece(0, 1, std::make_unique<Knight>(Color::White));
    placePiece(0, 2, std::make_unique<Bishop>(Color::White));
    placePiece(0, 3, std::make_unique<Queen>(Color::White));
    placePiece(0, 4, std::make_unique<King>(Color::White));
    placePiece(0, 5, std::make_unique<Bishop>(Color::White));
    placePiece(0, 6, std::make_unique<Knight>(Color::White));
    placePiece(0, 7, std::make_unique<Rook>(Color::White));
    for (int c = 0; c < 8; ++c)
        placePiece(1, c, std::make_unique<Pawn>(Color::White));

    // Чёрные фигуры
    placePiece(7, 0, std::make_unique<Rook>(Color::Black));
    placePiece(7, 1, std::make_unique<Knight>(Color::Black));
    placePiece(7, 2, std::make_unique<Bishop>(Color::Black));
    placePiece(7, 3, std::make_unique<Queen>(Color::Black));
    placePiece(7, 4, std::make_unique<King>(Color::Black));
    placePiece(7, 5, std::make_unique<Bishop>(Color::Black));
    placePiece(7, 6, std::make_unique<Knight>(Color::Black));
    placePiece(7, 7, std::make_unique<Rook>(Color::Black));
    for (int c = 0; c < 8; ++c)
        placePiece(6, c, std::make_unique<Pawn>(Color::Black));

    whiteKingsideCastle_ = true;
    whiteQueensideCastle_ = true;
    blackKingsideCastle_ = true;
    blackQueensideCastle_ = true;
    enPassantTarget_ = std::nullopt;
    halfmoveClock_ = 0;
    positionHistory_.clear();
}

void Board::display(bool flipped) const {
    std::cout << "\n";
    if (!flipped) {
        for (int r = 7; r >= 0; --r) {
            std::cout << " " << (r + 1) << " ";
            for (int c = 0; c < 8; ++c) {
                if (grid_[r][c]) {
                    std::cout << " " << grid_[r][c]->getSymbol() << " ";
                } else {
                    std::cout << " . ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "    a  b  c  d  e  f  g  h\n\n";
    } else {
        for (int r = 0; r < 8; ++r) {
            std::cout << " " << (r + 1) << " ";
            for (int c = 7; c >= 0; --c) {
                if (grid_[r][c]) {
                    std::cout << " " << grid_[r][c]->getSymbol() << " ";
                } else {
                    std::cout << " . ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "    h  g  f  e  d  c  b  a\n\n";
    }
}

const Piece* Board::getPiece(const Square& sq) const {
    if (!sq.isValid()) return nullptr;
    return grid_[sq.row][sq.col].get();
}

bool Board::canCastleKingside(Color c) const {
    return c == Color::White ? whiteKingsideCastle_ : blackKingsideCastle_;
}

bool Board::canCastleQueenside(Color c) const {
    return c == Color::White ? whiteQueensideCastle_ : blackQueensideCastle_;
}

// Проверка, атакована ли клетка фигурами данного цвета
// Проверяем от целевой клетки наружу — эффективнее полной генерации ходов
bool Board::isSquareAttackedBy(const Square& sq, Color byColor) const {
    // Проверка атаки пешкой
    int pawnDir = (byColor == Color::White) ? -1 : 1; // откуда атакует пешка
    for (int dc : {-1, 1}) {
        Square pawnSq{sq.row + pawnDir, sq.col + dc};
        if (pawnSq.isValid()) {
            const auto* p = getPiece(pawnSq);
            if (p && p->color == byColor && p->type == PieceType::Pawn) {
                return true;
            }
        }
    }

    // Проверка атаки конём
    static const std::vector<std::pair<int,int>> knightOffsets = {
        {2,1},{2,-1},{-2,1},{-2,-1},{1,2},{1,-2},{-1,2},{-1,-2}
    };
    for (auto [dr, dc] : knightOffsets) {
        Square knightSq{sq.row + dr, sq.col + dc};
        if (knightSq.isValid()) {
            const auto* p = getPiece(knightSq);
            if (p && p->color == byColor && p->type == PieceType::Knight) {
                return true;
            }
        }
    }

    // Проверка атаки королём
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            Square kingSq{sq.row + dr, sq.col + dc};
            if (kingSq.isValid()) {
                const auto* p = getPiece(kingSq);
                if (p && p->color == byColor && p->type == PieceType::King) {
                    return true;
                }
            }
        }
    }

    // Проверка атаки скользящими фигурами (ладья/ферзь по линиям)
    static const std::vector<std::pair<int,int>> rookDirs = {{1,0},{-1,0},{0,1},{0,-1}};
    for (auto [dr, dc] : rookDirs) {
        for (int i = 1; i < 8; ++i) {
            Square s{sq.row + dr * i, sq.col + dc * i};
            if (!s.isValid()) break;
            const auto* p = getPiece(s);
            if (p) {
                if (p->color == byColor &&
                    (p->type == PieceType::Rook || p->type == PieceType::Queen)) {
                    return true;
                }
                break; // фигура блокирует дальнейшую проверку
            }
        }
    }

    // Проверка атаки скользящими фигурами (слон/ферзь по диагоналям)
    static const std::vector<std::pair<int,int>> bishopDirs = {{1,1},{1,-1},{-1,1},{-1,-1}};
    for (auto [dr, dc] : bishopDirs) {
        for (int i = 1; i < 8; ++i) {
            Square s{sq.row + dr * i, sq.col + dc * i};
            if (!s.isValid()) break;
            const auto* p = getPiece(s);
            if (p) {
                if (p->color == byColor &&
                    (p->type == PieceType::Bishop || p->type == PieceType::Queen)) {
                    return true;
                }
                break;
            }
        }
    }

    return false;
}

bool Board::isInCheck(Color side) const {
    Square king = findKing(side);
    return isSquareAttackedBy(king, oppositeColor(side));
}

Square Board::findKing(Color side) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const auto* p = grid_[r][c].get();
            if (p && p->type == PieceType::King && p->color == side) {
                return {r, c};
            }
        }
    }
    // Не должно произойти в корректной игре
    return {-1, -1};
}

Board Board::copyForTest() const {
    Board copy;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (grid_[r][c]) {
                copy.grid_[r][c] = grid_[r][c]->clone();
            }
        }
    }
    copy.whiteKingsideCastle_ = whiteKingsideCastle_;
    copy.whiteQueensideCastle_ = whiteQueensideCastle_;
    copy.blackKingsideCastle_ = blackKingsideCastle_;
    copy.blackQueensideCastle_ = blackQueensideCastle_;
    copy.enPassantTarget_ = enPassantTarget_;
    copy.halfmoveClock_ = halfmoveClock_;
    // positionHistory_ не копируем — не нужна для проверки легальности
    return copy;
}

bool Board::isMoveLegal(const Move& move, Color side) const {
    // Проверяем, что на клетке "от" стоит фигура нужного цвета
    const auto* piece = getPiece(move.from);
    if (!piece || piece->color != side) return false;

    // Генерируем псевдолегальные ходы этой фигуры
    auto pseudoMoves = piece->generatePseudoLegalMoves(move.from, *this);

    // Ищем наш ход среди псевдолегальных
    bool found = false;
    for (const auto& pm : pseudoMoves) {
        if (pm.to == move.to && pm.promotion == move.promotion) {
            found = true;
            break;
        }
    }
    if (!found) return false;

    // Рокировка — дополнительная проверка: король не под шахом, промежуточные поля не атакованы
    if (piece->type == PieceType::King) {
        int colDiff = move.to.col - move.from.col;
        if (std::abs(colDiff) == 2) {
            // Рокировка
            if (isInCheck(side)) return false; // нельзя рокировать под шахом

            // Проверяем промежуточное поле
            int step = (colDiff > 0) ? 1 : -1;
            Square intermediate{move.from.row, move.from.col + step};
            if (isSquareAttackedBy(intermediate, oppositeColor(side))) {
                return false;
            }

            // Проверяем целевое поле
            if (isSquareAttackedBy(move.to, oppositeColor(side))) {
                return false;
            }

            return true; // рокировка не может оставить короля под шахом
        }
    }

    // Делаем копию доски и проверяем, что после хода король не под шахом
    Board test = copyForTest();
    test.makeMove(move);
    if (test.isInCheck(side)) return false;

    return true;
}

std::vector<Move> Board::getLegalMoves(Color side) const {
    std::vector<Move> legalMoves;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const auto* piece = grid_[r][c].get();
            if (!piece || piece->color != side) continue;

            Square pos{r, c};
            auto pseudoMoves = piece->generatePseudoLegalMoves(pos, *this);
            for (const auto& move : pseudoMoves) {
                if (isMoveLegal(move, side)) {
                    legalMoves.push_back(move);
                }
            }
        }
    }
    return legalMoves;
}

void Board::makeMove(const Move& move) {
    auto& fromCell = grid_[move.from.row][move.from.col];
    auto& toCell = grid_[move.to.row][move.to.col];

    bool isPawn = fromCell && fromCell->type == PieceType::Pawn;
    bool isCapture = (toCell != nullptr);

    // En passant — взятие на проходе
    if (isPawn && enPassantTarget_.has_value() && move.to == enPassantTarget_.value()) {
        // Удаляем захваченную пешку
        int capturedRow = move.from.row; // пешка стоит на том же ряду, что и наша
        grid_[capturedRow][move.to.col] = nullptr;
        isCapture = true;
    }

    // Рокировка — перемещаем ладью
    if (fromCell && fromCell->type == PieceType::King) {
        int colDiff = move.to.col - move.from.col;
        if (std::abs(colDiff) == 2) {
            // Перемещаем ладью
            if (colDiff > 0) {
                // Короткая рокировка
                grid_[move.from.row][5] = std::move(grid_[move.from.row][7]);
                grid_[move.from.row][5]->moved_ = true;
            } else {
                // Длинная рокировка
                grid_[move.from.row][3] = std::move(grid_[move.from.row][0]);
                grid_[move.from.row][3]->moved_ = true;
            }
        }
    }

    // Обновление прав рокировки
    if (fromCell) {
        if (fromCell->type == PieceType::King) {
            if (fromCell->color == Color::White) {
                whiteKingsideCastle_ = false;
                whiteQueensideCastle_ = false;
            } else {
                blackKingsideCastle_ = false;
                blackQueensideCastle_ = false;
            }
        }
        if (fromCell->type == PieceType::Rook) {
            if (fromCell->color == Color::White) {
                if (move.from.row == 0 && move.from.col == 0) whiteQueensideCastle_ = false;
                if (move.from.row == 0 && move.from.col == 7) whiteKingsideCastle_ = false;
            } else {
                if (move.from.row == 7 && move.from.col == 0) blackQueensideCastle_ = false;
                if (move.from.row == 7 && move.from.col == 7) blackKingsideCastle_ = false;
            }
        }
    }
    // Если ладья съедена — тоже отнимаем право рокировки
    if (move.to.row == 0 && move.to.col == 0) whiteQueensideCastle_ = false;
    if (move.to.row == 0 && move.to.col == 7) whiteKingsideCastle_ = false;
    if (move.to.row == 7 && move.to.col == 0) blackQueensideCastle_ = false;
    if (move.to.row == 7 && move.to.col == 7) blackKingsideCastle_ = false;

    // Обновление en passant target
    if (isPawn && std::abs(move.to.row - move.from.row) == 2) {
        enPassantTarget_ = Square{(move.from.row + move.to.row) / 2, move.from.col};
    } else {
        enPassantTarget_ = std::nullopt;
    }

    // Обновление счётчика полуходов (правило 50 ходов)
    if (isPawn || isCapture) {
        halfmoveClock_ = 0;
    } else {
        halfmoveClock_++;
    }

    // Перемещение фигуры
    fromCell->moved_ = true;
    toCell = std::move(fromCell);

    // Превращение пешки
    if (toCell->type == PieceType::Pawn && move.promotion != '\0') {
        Color col = toCell->color;
        switch (move.promotion) {
            case 'q': toCell = std::make_unique<Queen>(col); break;
            case 'r': toCell = std::make_unique<Rook>(col); break;
            case 'b': toCell = std::make_unique<Bishop>(col); break;
            case 'n': toCell = std::make_unique<Knight>(col); break;
            default: toCell = std::make_unique<Queen>(col); break;
        }
        toCell->moved_ = true;
    }
}

// FEN-подобная строка позиции для определения троекратного повторения
std::string Board::getPositionKey(Color sideToMove) const {
    std::ostringstream oss;

    // Позиция фигур
    for (int r = 7; r >= 0; --r) {
        int emptyCount = 0;
        for (int c = 0; c < 8; ++c) {
            if (grid_[r][c]) {
                if (emptyCount > 0) {
                    oss << emptyCount;
                    emptyCount = 0;
                }
                oss << grid_[r][c]->fenChar();
            } else {
                emptyCount++;
            }
        }
        if (emptyCount > 0) oss << emptyCount;
        if (r > 0) oss << '/';
    }

    // Сторона хода
    oss << ' ' << (sideToMove == Color::White ? 'w' : 'b');

    // Права рокировки
    oss << ' ';
    std::string castling;
    if (whiteKingsideCastle_) castling += 'K';
    if (whiteQueensideCastle_) castling += 'Q';
    if (blackKingsideCastle_) castling += 'k';
    if (blackQueensideCastle_) castling += 'q';
    if (castling.empty()) castling = "-";
    oss << castling;

    // En passant
    oss << ' ';
    if (enPassantTarget_.has_value()) {
        oss << enPassantTarget_->toString();
    } else {
        oss << '-';
    }

    return oss.str();
}

GameState Board::evaluateGameState(Color sideToMove) {
    auto legalMoves = getLegalMoves(sideToMove);

    if (legalMoves.empty()) {
        if (isInCheck(sideToMove)) {
            return GameState::Checkmate;
        } else {
            return GameState::Stalemate;
        }
    }

    // Правило 50 ходов
    if (halfmoveClock_ >= 100) { // 100 полуходов = 50 ходов
        return GameState::DrawBy50Moves;
    }

    // Троекратное повторение
    std::string currentKey = getPositionKey(sideToMove);
    int count = 0;
    for (const auto& key : positionHistory_) {
        if (key == currentKey) count++;
    }
    if (count >= 2) { // текущая + 2 предыдущих = 3
        return GameState::DrawByRepetition;
    }

    // Сохраняем текущую позицию в историю
    positionHistory_.push_back(currentKey);

    return GameState::InProgress;
}
