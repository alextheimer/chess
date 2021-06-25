#ifndef GAME_GAME_H_
#define GAME_GAME_H_

#include <ostream>
#include <unordered_map>

#include "board/board.h"
#include "game/move.h"

namespace game {

extern const std::unordered_map<board::Square, board::Piece> INIT_PIECE_MAP;

class Player {
 public:
    virtual Move getMove(const board::Board& board) = 0;
};

class Game {
 private:
    board::Board& board_;
    Player& white_player_;
    Player& black_player_;
    board::PieceColor next_player_;
 public:
    Game(board::Board& board, Player& white_player, Player& black_player);
    void render(std::ostream& ostream);
    void runPly();
    bool isEnded();
    Player& getWinner();
};

}

#endif  // GAME_GAME_H_
