// Copyright 2021 Alex Theimer

#ifndef GAME_GAME_H_
#define GAME_GAME_H_

#include <ostream>
#include <unordered_map>

#include "board/board.h"
#include "game/move.h"

namespace game {

/*
Thrown by Game when a Player attempts an invalid move.
*/
class InvalidMoveEx : public std::exception {
 public:
    InvalidMoveEx(Move move);
    const char* what() const throw();
    Move getMove() const;
 private:
    Move move_;
};

// Square->Piece map for initialization of a standard game of chess.
extern const std::unordered_map<board::Square, board::Piece> INIT_PIECE_MAP;

class Player {
 public:
    virtual Move getMove(const board::Board& board, board::PieceColor) = 0;
};

// Manages a game of chess between two Players.
class Game {
 public:
    /*
    Instantiates a game of standard chess.

    Note: this intentionally accepts player pointers because there is no
          explicit guarantee that the players will not be modified.
    */
    Game(board::Board* board, Player* white_player, Player* black_player);

    void renderBoard(std::ostream& ostream) const;

    // TODO(theimer): C++ way to denote throws InvalidMoveEx?
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
    board::Board* board_;
    Player* white_player_;
    Player* black_player_;
    board::PieceColor next_player_color_;
};

}  // namespace game

#endif  // GAME_GAME_H_
