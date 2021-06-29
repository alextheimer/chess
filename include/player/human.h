// Copyright 2021 Alex Theimer

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

}  // namespace player

#endif  // PLAYER_HUMAN_H_
