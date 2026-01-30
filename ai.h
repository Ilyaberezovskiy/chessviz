#ifndef AI_H
#define AI_H

#include "board.h"
#include "move.h"
#include "pieces.h"

// Оценка позиции с точки зрения белых
int evaluateBoard(const Board& board);

// Поиск лучшего хода для заданной стороны (minimax + alpha-beta, глубина 4)
Move findBestMove(Board& board, Color side);

#endif
