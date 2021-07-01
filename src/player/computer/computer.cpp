#include "player/computer/computer.h"

#include "player/computer/search.h"

using board::Board;
using board::PieceColor;
using game::Move;

player::Computer::Computer(std::string name) : Player(name) {
    // intentionally blank
}

Move player::Computer::getMove(const Board& board, PieceColor color) {
    return player::computer::alphaBetaSearch(
                                  board, color, 5 /* max depth */,
                                  &player::computer::basicBoardHeuristic);
}
