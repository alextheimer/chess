#ifndef PLAYER_COMPUTER_H_
#define PLAYER_COMPUTER_H_

#include <string>

#include "game/game.h"
#include "util/fixedmap.h"
#include "player/computer/scorecache.h"
#include "player/computer/search.h"

namespace player {

namespace computer {

class ScoreCacheImpl : public util::FixedMap<std::size_t,
                                             player::computer::BoardScore>,
                       public player::computer::ScoreCache {
 public:
    ScoreCacheImpl(std::size_t size);
    player::computer::BoardScore* end() const;
    player::computer::BoardScore* find(std::size_t key) const;
    void set(std::size_t key, const player::computer::BoardScore& value);
};

}  // namespace computer

class Computer : public game::Player {
 public:
    Computer(std::string name);
    game::Move getMove(const board::Board& board, board::PieceColor);

 private:
    player::computer::ScoreCacheImpl score_cache_;
};

}  // namespace player

#endif  // PLAYER_COMPUTER_H_
