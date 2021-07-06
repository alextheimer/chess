// Copyright 2021 Alex Theimer

#ifndef BOARD_ZOBHASH_H_
#define BOARD_ZOBHASH_H_

#include "board/square.h"
#include "board/piece.h"

/*
################################################################################
                          ~~~ Zobrist Hashing ~~~

    Use these functions to incrementally update the hash value of a Board.
    Piece/Square pairs are either "included" in or "excluded" from the
    hash value.

################################################################################
*/

namespace board {

// Hash value of a Board with no included Piece/Square pairs.
constexpr std::size_t ZOB_INIT = 0;

/*
Includes/Excludes a Piece/Square pair from a hash value.
*/
std::size_t toggleZobPiece(std::size_t hash, Piece piece, SquareIndex square_index);

// TODO(theimer): use this!
/*
Excludes a Piece/Square pair from a hash value, and includes
a Piece/Square pair where the second Square is identical to the first.
*/
std::size_t replaceZobPiece(std::size_t hash, Piece old_piece,
                        Piece new_piece, SquareIndex square_index);

}  // namespace board

#endif  // BOARD_ZOBHASH_H_
