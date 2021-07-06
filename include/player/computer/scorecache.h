#ifndef PLAYER_COMPUTER_SCORECACHE_H_
#define PLAYER_COMPUTER_SCORECACHE_H_

#include <cstdint>

namespace player {
namespace computer{

typedef int64_t BoardScore;

class ScoreCache {
 public:
    virtual BoardScore* end() const  = 0;
    virtual BoardScore* find(std::size_t key) const = 0;
    virtual void set(std::size_t key, const BoardScore& value) = 0;
};

}  // namespace computer
}  // namespace player

#endif  // PLAYER_COMPUTER_SCORECACHE_H_
