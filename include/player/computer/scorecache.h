#ifndef PLAYER_COMPUTER_SCORECACHE_H_
#define PLAYER_COMPUTER_SCORECACHE_H_

#include <cstdint>

#include "board/board.h"

namespace player {
namespace computer{

typedef int64_t BoardScore;

/*
Interface of a data structure that allows fast storage/retrieval of BoardScores.
BoardScores are keyed on Board-depth pairs.
*/
class IScoreCache {
 public:
    /*
    Returns an iterator referring to the "past-the-end" BoardScore.

    The iterator doesn't point to any BoardScore and should not be dereferenced.
    */
    virtual BoardScore* end() const  = 0;

    /*
    Returns a pointer to the BoardScore if the Board-depth pair
    exists in the map.

    Otherwise, returns the end iterator (i.e. the return value of end()).

    @param depth: must be >= 0
    */
    virtual BoardScore* find(const board::Board& board,
                             std::size_t depth) const = 0;
    /*
    Sets the BoardScore at the Board-depth pair.

    @param depth: must be >= 0
    */
    virtual void set(const board::Board& board, std::size_t depth,
                     BoardScore value) = 0;
};

}  // namespace computer
}  // namespace player

#endif  // PLAYER_COMPUTER_SCORECACHE_H_
