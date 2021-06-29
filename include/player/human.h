#ifndef PLAYER_HUMAN_H_
#define PLAYER_HUMAN_H_

#include "game/game.h"

namespace player {

class Human : public game::Player {
 public:
    game::Move getMove(const board::Board& board);

 private:
    // TODO(theimer): any state?
};

}

#endif  // PLAYER_HUMAN_H_
