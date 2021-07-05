#ifndef PLAYER_COMPUTER_H_
#define PLAYER_COMPUTER_H_

#include <string>

#include "game/game.h"
#include "util/fixedmap.h"

namespace player {

class Computer : public game::Player {
 public:
    Computer(std::string name);
    game::Move getMove(const board::Board& board, board::PieceColor);

 private:
    util::FixedMap<int64_t> score_cache_;
};

}  // namespace player

#endif  // PLAYER_COMPUTER_H_
