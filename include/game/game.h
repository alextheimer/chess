#ifndef GAME_GAME_H_
#define GAME_GAME_H_

#include <ostream>
#include <unordered_map>

#include "board/board.h"
#include "game/move.h"

namespace game {

// Square->Piece map for initialization of a standard game of chess.
extern const std::unordered_map<board::Square, board::Piece> INIT_PIECE_MAP;

class Player {
 public:
    virtual Move getMove(const board::Board& board) = 0;
};

// Manages a game of chess between two Players.
class Game {
 public:
    Game(board::Board& board, Player& white_player, Player& black_player);
    void render(std::ostream& ostream) const;  // TODO(theimer): should just be a toString

    /*
    Executes a player's turn.
    */
    void runPly();

    /*
    Returns true iff the game is over.
    */
    bool isEnded() const;

    /*
    Returns the winning Player instance.
    The game must be ended before this member is called.
    */
    Player& getWinner() const;

 private:
    board::Board& board_;
    Player& white_player_;
    Player& black_player_;
    board::PieceColor next_player_;  // TODO(theimer): rename to next_player_color_
};

}

#endif  // GAME_GAME_H_
