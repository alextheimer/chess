#ifndef GAME_MOVE_H_
#define GAME_MOVE_H_

#include "board/board.h"

#include <optional>

namespace game {

struct Move {
    board::Square from;
    board::Square to;
};

bool operator==(Move lhs, Move rhs);
std::ostream& operator<<(std::ostream& out, Move move);

/*
Fills a buffer with all possible moves for the Piece at the specified Square.
@param square: must be occupied by a Piece of PieceColor `color`
@param buffer: an Iterator at the start of the buffer.
@return: the number of Moves added to the buffer.
*/
std::size_t getPieceMoves(const board::Board& board, board::PieceColor color,
                          board::Square square, Move* buffer);

/*
Fills a buffer with all possible moves for Pieces of the specified color.
@param buffer: an Iterator at the start of the buffer.
@return: the number of Moves added to the buffer.
*/
std::size_t getAllMoves(const board::Board& board, board::PieceColor color, Move* buffer);

/*
Applies the specified move to the board.
@param move: move.from must be occupied;
             move.to must be unoccupied or occupied by a piece with a
                 different color than the piece at move.from.
*/
void makeMove(board::Board& board, Move move);

/*
Reverses a move, then sets a replacement Piece at the move's `to` Square.
@param move: move.from must be unoccupied;
             move.to must be occupied.
@param replacement: must have color different from Piece at move.to
*/
void unmakeMove(board::Board& board, Move move, board::Piece replacement);

}  // namespace game

namespace std {

    std::string to_string(game::Move move);

}  // namespace std

#endif  // GAME_MOVE_H_
