#include "player/computer/computer.h"

#include "player/computer/search.h"

using board::Board;
using board::PieceColor;
using game::Move;

static constexpr std::size_t CACHE_SIZE = 1000000;
static constexpr std::size_t SEARCH_DEPTH = 6;

player::Computer::Computer(std::string name) :
        score_cache_(CACHE_SIZE),
        Player(name) {
    // intentionally blank
}

Move player::Computer::getMove(const Board& board, PieceColor color) {
    return player::computer::alphaBetaSearch(
                                  board, color, SEARCH_DEPTH,
                                  &player::computer::basicBoardHeuristic,
                                  &score_cache_);
}
