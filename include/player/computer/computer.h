#ifndef PLAYER_COMPUTER_H_
#define PLAYER_COMPUTER_H_

#include <string>
#include <unordered_map>

#include "game/game.h"

namespace player {

class Computer : public game::Player {
 public:
    Computer(std::string name);
    game::Move getMove(const board::Board& board, board::PieceColor);

 private:
    std::unordered_map<board::ZobHash, int64_t> score_cache_;
};

}  // namespace player

#endif  // PLAYER_COMPUTER_H_
