#include "ai.h"
#include <algorithm>
#include <limits>

// --- Piece-Square Tables (с точки зрения белых, row 0 = rank 1) ---

static const int PAWN_TABLE[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0},
    {  5, 10, 10,-20,-20, 10, 10,  5},
    {  5, -5,-10,  0,  0,-10, -5,  5},
    {  0,  0,  0, 20, 20,  0,  0,  0},
    {  5,  5, 10, 25, 25, 10,  5,  5},
    { 10, 10, 20, 30, 30, 20, 10, 10},
    { 50, 50, 50, 50, 50, 50, 50, 50},
    {  0,  0,  0,  0,  0,  0,  0,  0}
};

static const int KNIGHT_TABLE[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
};

static const int BISHOP_TABLE[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20}
};

static const int ROOK_TABLE[8][8] = {
    {  0,  0,  0,  5,  5,  0,  0,  0},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    {  5, 10, 10, 10, 10, 10, 10,  5},
    {  0,  0,  0,  0,  0,  0,  0,  0}
};

static int getPieceValue(PieceType type) {
    switch (type) {
        case PieceType::Pawn:   return 100;
        case PieceType::Knight: return 320;
        case PieceType::Bishop: return 330;
        case PieceType::Rook:   return 500;
        case PieceType::Queen:  return 900;
        case PieceType::King:   return 20000;
    }
    return 0;
}

static int getPieceSquareBonus(const Piece* piece, int row, int col) {
    int r = (piece->color == Color::White) ? row : (7 - row);
    int c = col;

    switch (piece->type) {
        case PieceType::Pawn:   return PAWN_TABLE[r][c];
        case PieceType::Knight: return KNIGHT_TABLE[r][c];
        case PieceType::Bishop: return BISHOP_TABLE[r][c];
        case PieceType::Rook:   return ROOK_TABLE[r][c];
        default: return 0;
    }
}

int evaluateBoard(const Board& board) {
    int score = 0;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Piece* piece = board.getPiece({row, col});
            if (!piece) continue;

            int value = getPieceValue(piece->type) + getPieceSquareBonus(piece, row, col);

            if (piece->color == Color::White) {
                score += value;
            } else {
                score -= value;
            }
        }
    }

    return score;
}

// --- Minimax с alpha-beta отсечением ---

static int minimax(Board& board, int depth, int alpha, int beta, bool maximizing, Color side) {
    if (depth == 0) {
        return evaluateBoard(board);
    }

    std::vector<Move> moves = board.getLegalMoves(side);

    // Проверка на конец игры
    if (moves.empty()) {
        if (board.isInCheck(side)) {
            // Мат: чем быстрее, тем лучше (или хуже для проигравшего)
            return maximizing ? (-100000 - depth) : (100000 + depth);
        }
        return 0; // Пат
    }

    // Сортировка: взятия первыми для лучшего alpha-beta отсечения
    std::sort(moves.begin(), moves.end(), [&board](const Move& a, const Move& b) {
        bool aCapture = board.getPiece(a.to) != nullptr;
        bool bCapture = board.getPiece(b.to) != nullptr;
        return aCapture > bCapture;
    });

    if (maximizing) {
        int maxEval = std::numeric_limits<int>::min();
        for (const auto& move : moves) {
            Board copy = board.copyForTest();
            copy.makeMove(move);
            int eval = minimax(copy, depth - 1, alpha, beta, false, oppositeColor(side));
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const auto& move : moves) {
            Board copy = board.copyForTest();
            copy.makeMove(move);
            int eval = minimax(copy, depth - 1, alpha, beta, true, oppositeColor(side));
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

Move findBestMove(Board& board, Color side) {
    std::vector<Move> moves = board.getLegalMoves(side);

    // Сортировка: взятия первыми
    std::sort(moves.begin(), moves.end(), [&board](const Move& a, const Move& b) {
        bool aCapture = board.getPiece(a.to) != nullptr;
        bool bCapture = board.getPiece(b.to) != nullptr;
        return aCapture > bCapture;
    });

    bool maximizing = (side == Color::White);
    int bestScore = maximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    Move bestMove = moves[0];

    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    for (const auto& move : moves) {
        Board copy = board.copyForTest();
        copy.makeMove(move);
        int score = minimax(copy, 3, alpha, beta, !maximizing, oppositeColor(side));

        if (maximizing) {
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
            alpha = std::max(alpha, score);
        } else {
            if (score < bestScore) {
                bestScore = score;
                bestMove = move;
            }
            beta = std::min(beta, score);
        }
    }

    return bestMove;
}
