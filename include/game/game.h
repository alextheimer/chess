#ifndef GAME_GAME_H_
#define GAME_GAME_H_

#include <ostream>

#include "game/player.h"
#include "board/board.h"

using namespace game;

namespace game {

class Game {
 private:
    Board& board_;
    Player& white_player_;
    Player& black_player_;
    PieceColor next_player_;
 public:
    Game(Board& board, Player& white_player, Player& black_player);
    void render(std::ostream& ostream);
    void runPly();
    bool isEnded();
    Player& getWinner();
};

}

#endif  // GAME_GAME_H_
