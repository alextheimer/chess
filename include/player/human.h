// Copyright 2021 Alex Theimer

#ifndef PLAYER_HUMAN_H_
#define PLAYER_HUMAN_H_

#include <string>

#include "game/game.h"

namespace player {

class Human : public game::Player {
 public:
    Human(std::string name);
    game::Move getMove(const board::Board& board, board::PieceColor color);
};

}  // namespace player

#endif  // PLAYER_HUMAN_H_
