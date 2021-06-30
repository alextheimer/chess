#ifndef PLAYER_COMPUTER_H_
#define PLAYER_COMPUTER_H_

#include "game/game.h"

namespace player {

class Computer : public game::Player {
 public:
    game::Move getMove(const board::Board& board, board::PieceColor);

 // TODO(theimer): private cache fields
};

}  // namespace player

#endif  // PLAYER_COMPUTER_H_
