#ifndef PLAYER_COMPUTER_SCORECACHE_H_
#define PLAYER_COMPUTER_SCORECACHE_H_

#include <cstdint>

#include "board/board.h"

namespace player {
namespace computer{

typedef int64_t BoardScore;

/*
Data structure that allows fast storage/retrieval of BoardScores.
TODO(theimer): expand
*/
class ScoreCache {
 public:
    virtual BoardScore* end() const  = 0;
    virtual BoardScore* find(const board::Board& board,
                             std::size_t depth) const = 0;
    virtual void set(const board::Board& board, std::size_t key,
                     BoardScore value) = 0;
};

}  // namespace computer
}  // namespace player

#endif  // PLAYER_COMPUTER_SCORECACHE_H_
