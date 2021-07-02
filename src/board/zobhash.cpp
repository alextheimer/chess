#include "board/zobhash.h"

#include <vector>

#include "util/rand.h"
#include "util/math.h"

using board::ZobHash;
using board::Piece;
using board::PieceColor;
using board::PieceType;
using board::Square;
using board::Board;

/*
################################################################################
                        =============================
                        =====  Zobrist Hashing  =====
                        =============================

    Generate a random value for all Piece/Square pairs, then XOR these into
    an "aggregate" value as pieces are added to / removed from a Board.

    As long as these values contain sufficient entropy, the "aggregate" can
    be used as a Board's hash value.

    This implementation uses 64-bit strings and std::mt19937_64 to generate
    the Piece/Square pair values. These values are stored into a single
    std::vector, where a specific Square/Piece index is found by
    calling getZobIndex.

################################################################################
*/

/*
Fills a vector with a random 64-bit string for every valid
Square/PieceColor/PieceType combination.
*/
static std::vector<ZobHash> makeZobVec();

// TODO(theimer): reconcile this with the same info in board.cpp?
// TODO(theimer): make constexpr
static const std::size_t NUM_SQUARE_DIM_BITS = util::log2Ceil(Board::WIDTH);

// these only de-clutter the below code
static constexpr std::size_t NUM_TYPES =
            static_cast<std::size_t>(PieceType::NUM_PIECE_TYPES);
static constexpr std::size_t NUM_COLORS =
            static_cast<std::size_t>(PieceColor::NUM_PIECE_COLORS);

// TODO(theimer): just hard-code this.
// Contains a random 64-bit value for every Square/PieceColor/PieceType combo.
static const std::vector<ZobHash> ZOB_ARRAY = makeZobVec();

/*
Converts a Square to an index on [0, Board::SIZE).
The Square->Index mapping given by this function is bijective.
*/
static std::size_t squareToIndex(Square square) {
    return (static_cast<std::size_t>(square.row) << NUM_SQUARE_DIM_BITS)
           | (static_cast<std::size_t>(square.col));
}

/*
Returns an index into ZOB_ARRAY that's unique to a specific
(Square_index, Piece) pair.

@param square_index: the result of squareToIndex.
*/
static std::size_t getZobIndex(std::size_t square_index, Piece piece) {
    static constexpr std::size_t PER_SQUARE = NUM_TYPES * NUM_COLORS;
    static constexpr std::size_t PER_TYPE = NUM_COLORS;

    return (square_index * PER_SQUARE)
            + (static_cast<std::size_t>(piece.type) * PER_TYPE)
            + static_cast<std::size_t>(piece.color);
}

static std::vector<ZobHash> makeZobVec() {
    // just assigns a random 64-bit value to every index
    std::vector<ZobHash> zob_vec(Board::SIZE * NUM_TYPES * NUM_COLORS);
    for (std::size_t isquare = 0; isquare < Board::SIZE; ++isquare) {
        for (std::size_t itype = 0; itype < NUM_TYPES; ++itype) {
            for (std::size_t icolor = 0; icolor < NUM_COLORS; ++icolor) {
                PieceType type = static_cast<PieceType>(itype);
                PieceColor color = static_cast<PieceColor>(icolor);
                std::size_t index = getZobIndex(isquare, (Piece){type, color});
                zob_vec[index] = util::getRand64();
            }
        }
    }
    return zob_vec;
}

/*
Same as toggleZobPiece, but with "lower-level" parameters.
Prevents zob_index (and/or square_index) recomputation.
*/
ZobHash toggleZobIndex(ZobHash hash, std::size_t zob_index) {
    return hash ^ ZOB_ARRAY[zob_index];
}

ZobHash toggleZobPiece(ZobHash hash, Piece piece, Square square) {
    std::size_t square_index = squareToIndex(square);
    std::size_t zob_index = getZobIndex(square_index, piece);
    return toggleZobIndex(hash, zob_index);
}

ZobHash replaceZobPiece(ZobHash hash, Piece old_piece,
                        Piece new_piece, Square square) {
    std::size_t square_index = squareToIndex(square);
    std::size_t zob_index_old = getZobIndex(square_index, old_piece);
    std::size_t zob_index_new = getZobIndex(square_index, new_piece);
    hash = toggleZobIndex(hash, zob_index_old);
    return toggleZobIndex(hash, zob_index_new);
}
