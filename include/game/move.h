// Copyright 2021 Alex Theimer

#ifndef GAME_MOVE_H_
#define GAME_MOVE_H_

#include <optional>
#include <string>

#include "board/board.h"

namespace game {

// Max number of moves per ply; roughly sourced from:
// https://chess.stackexchange.com/
//     questions/4490/maximum-possible-movement-in-a-turn
constexpr std::size_t MAX_NUM_MOVES_PLY = 300;

struct Move {
    board::Square from;
    board::Square to;
};

bool operator==(Move lhs, Move rhs);
std::ostream& operator<<(std::ostream& out, Move move);

/*
Fills a buffer with all possible moves for the Piece at the specified Square.
@param square: must be occupied by a Piece of PieceColor `color`
@param buffer: a random-access iterator at the start of the buffer.
@return: the number of Moves added to the buffer.
*/
template<typename RandomAccessIter>
std::size_t getPieceMoves(const board::Board& board, board::PieceColor color,
                          board::Square square, RandomAccessIter buffer);

/*
Fills a buffer with all possible moves for Pieces of the specified color.
@param buffer: a random-access iterator at the start of the buffer.
@return: the number of Moves added to the buffer.
*/
template<typename RandomAccessIter>
std::size_t getAllMoves(const board::Board& board,
                        board::PieceColor color, RandomAccessIter buffer);

/*
Applies the specified move to the board.
@param move: move.from must be occupied;
             move.to must be unoccupied or occupied by a piece with a
                 different color than the piece at move.from.
@return: optional with overwritten piece (if applicable);
         empty optional otherwise
*/
std::optional<board::Piece> makeMove(board::Board* board, Move move);

/*
Reverses a move, then sets a replacement Piece at the move's `to` Square.
@param move: move.from must be unoccupied;
             move.to must be occupied.
@param replacement: if non-empty, must have color different
                    from Piece at move.to
*/
void unmakeMove(board::Board* board, Move move,
                std::optional<board::Piece> replacement);

/*
Returns true iff `move` is valid.
*/
bool isValidMove(const board::Board& board, board::PieceColor color, Move move);

}  // namespace game

namespace std {

    std::string to_string(game::Move move);

}  // namespace std

#endif  // GAME_MOVE_H_
