#ifndef GAME_PLAYER_H_
#define GAME_PLAYER_H_

#include "board/board.h"

using namespace board;

namespace game {

class Player {
 public:
    virtual void makeMove(const Board& board) = 0;
};

}

#endif  // GAME_PLAYER_H_
