#ifndef PLAYER_COMPUTER_SCORECACHE_H_
#define PLAYER_COMPUTER_SCORECACHE_H_

#include <cstdint>

namespace player {
namespace computer{

template <typename K, typename V>
class ScoreCache {
 public:
    virtual V* end() const  = 0;
    virtual V* find(const K& key) const = 0;
    virtual void set(const K& key, const V& value) = 0;
};

}  // namespace computer
}  // namespace player

#endif  // PLAYER_COMPUTER_SCORECACHE_H_
