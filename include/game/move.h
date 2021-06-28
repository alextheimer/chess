#ifndef GAME_MOVE_H_
#define GAME_MOVE_H_

#include "board/board.h"

#include <optional>

namespace game {

struct Move {
    board::Square from;
    board::Square to;
    std::string toString() const;
};

bool operator==(Move lhs, Move rhs);
std::ostream& operator<<(std::ostream& out, Move move);

// TODO(theimer): movegen members that aren't explicitly built for performance

/*
Fills a buffer with all possible moves for the Piece at the specified Square.
@param square: must be occupied by a Piece of PieceColor `color`
@param buffer: an Iterator at the start of the buffer.
@return: the number of Moves added to the buffer.
*/
std::size_t getPieceMoves(const board::Board& board, board::PieceColor color,
                          board::Square square, Move * buffer);

/*
Fills a buffer with all possible moves for Pieces of the specified color.
@param buffer: an Iterator at the start of the buffer.
@return: the number of Moves added to the buffer.
*/
std::size_t getAllMoves(const board::Board& board, board::PieceColor color, Move * buffer);

// TODO(theimer): need "overwrite" variant of makeMove
// TODO(theimer): need "pieceless" variant of unmakeMove

/*
Applies the specified move to the board. TODO
*/
void makeMove(board::Board& board, Move move);

/*
Reverses a move, then sets a replacement Piece at the move's `to` Square.
TODO: elaborate more when variants are more concrete.
*/
void unmakeMove(board::Board& board, Move move, board::Piece replacement);

}

#endif  // GAME_MOVE_H_
