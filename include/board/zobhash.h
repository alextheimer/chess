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

// Datatype of a Board's Zobrist Hash
typedef std::size_t ZobHash;

// Hash value of a Board with no included Piece/Square pairs.
constexpr ZobHash ZOB_INIT = 0;

/*
Includes/Excludes a Piece/Square pair from a hash value.
*/
ZobHash toggleZobPiece(ZobHash hash, Piece piece, SquareIndex square_index);

// TODO(theimer): use this!
/*
Excludes a Piece/Square pair from a hash value, and includes
a Piece/Square pair where the second Square is identical to the first.
*/
ZobHash replaceZobPiece(ZobHash hash, Piece old_piece,
                        Piece new_piece, SquareIndex square_index);

}  // namespace board

#endif  // BOARD_ZOBHASH_H_
