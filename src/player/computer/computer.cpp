#include "player/computer/computer.h"

#include "player/computer/search.h"

using board::Board;
using board::PieceColor;
using game::Move;

Move player::Computer::getMove(const Board& board, PieceColor color) {
    return player::computer::alphaBetaSearch(
                                  board, color, 5 /* max depth */,
                                  &player::computer::basicBoardHeuristic);
}
