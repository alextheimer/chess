#include "player/computer/computer.h"

#include "player/computer/search.h"

using board::Board;
using board::PieceColor;
using game::Move;
using player::computer::BoardScore;
using player::computer::ScoreCache;
using player::computer::ScoreCacheImpl;

static constexpr std::size_t CACHE_SIZE = 1000000;
static constexpr std::size_t SEARCH_DEPTH = 6;

ScoreCacheImpl::ScoreCacheImpl(std::size_t size) :
        util::FixedMap<std::size_t, BoardScore>(size) {
    // intentionally blank
}

BoardScore* ScoreCacheImpl::end() const {
    return util::FixedMap<std::size_t, BoardScore>::end();
}

BoardScore* ScoreCacheImpl::find(std::size_t key) const {
    return util::FixedMap<std::size_t, BoardScore>::find(key);
}

void ScoreCacheImpl::set(std::size_t key, const BoardScore& value) {
    return util::FixedMap<std::size_t, BoardScore>::set(key, value);
}

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
