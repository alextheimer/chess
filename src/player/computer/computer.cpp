#include "player/computer/computer.h"

#include "player/computer/search.h"
#include "util/assert.h"

using board::Board;
using board::PieceColor;
using game::Move;
using player::computer::BoardScore;
using player::computer::ScoreCache;
using player::Computer;

typedef util::FixedSizeMap<std::size_t, BoardScore> BaseMap;

static constexpr std::size_t CACHE_SIZE = 1000000;
static constexpr std::size_t SEARCH_DEPTH = 6;

std::size_t hashWithDepth(const Board& board, std::size_t depth) {
    ASSERT(depth >= 0,
            "depth must be at least 0; depth: " + std::to_string(depth));
    // TODO(theimer): something more clever (also use std::hash)
    return board.getZobHash() + depth;
}

Computer::ScoreCacheImpl::ScoreCacheImpl(std::size_t size) :
                                         BaseMap(size) {
    // intentionally blank
}

BoardScore* Computer::ScoreCacheImpl::end() const {
    return BaseMap::end();
}

BoardScore* Computer::ScoreCacheImpl::find(const Board& board,
                                           std::size_t depth) const {
    ASSERT(depth >= 0,
            "depth must be at least 0; depth: " + std::to_string(depth));
    std::size_t hash_with_depth = hashWithDepth(board, depth);
    return BaseMap::find(hash_with_depth);
}

void Computer::ScoreCacheImpl::set(const Board& board, std::size_t depth,
                                   BoardScore value) {
    ASSERT(depth >= 0,
            "depth must be at least 0; depth: " + std::to_string(depth));
    std::size_t hash_with_depth = hashWithDepth(board, depth);
    BaseMap::set(hash_with_depth, value);
}

Computer::Computer(std::string name) :
        score_cache_(CACHE_SIZE),
        Player(name) {
    // intentionally blank
}

Move Computer::getMove(const Board& board, PieceColor color) {
    return player::computer::alphaBetaSearch(
                                  board, color, SEARCH_DEPTH,
                                  &player::computer::basicBoardHeuristic,
                                  &score_cache_);
}
