#ifndef PLAYER_HUMAN_H_
#define PLAYER_HUMAN_H_

#include "game/game.h"

namespace player {

class Human : public game::Player {
 private:
    // TODO(theimer): any state?
 public:
    game::Move getMove(const board::Board& board);
};

}

#endif  // PLAYER_HUMAN_H_
