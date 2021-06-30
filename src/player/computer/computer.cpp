#include "player/computer/computer.h"

#include "player/computer/search.h"

using board::Board;
using board::PieceColor;
using game::Move;

Move player::Computer::getMove(const Board& board, PieceColor color) {
    // TODO(theimer): copy should be made in the actual search
    Board board_copy(board);
    return player::computer::alphaBetaSearch(&board_copy, color, 5, &player::computer::basicBoardHeuristic);
}
