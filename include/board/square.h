#ifndef BOARD_SQUARE_H_
#define BOARD_SQUARE_H_

#include <cstdint>
#include <ostream>

namespace board {

// datatype used for Board row/col indices
typedef uint8_t DimIndex;

// datatype for indices on [0, 64).
// See Square::squareToIndex for details.
typedef uint8_t SquareIndex;

// TODO(theimer): make Square/Piece/Move immutable

struct Square {
    // this is really just a POD type, but I want to enforce
    //     constructor initialization to support row/col assertions.

    // the maximum (exclusive) value either DimIndex of a square is allowed
    static constexpr std::size_t MAX_DIM_VALUE = 8;
    static constexpr std::size_t NUM_SQUARES = MAX_DIM_VALUE * MAX_DIM_VALUE;

    DimIndex row;
    DimIndex col;

    /*
    row and col must each lie on [0, Board::WIDTH).
    */
    Square(DimIndex row, DimIndex col);

    // TODO(theimer): describe size_t params

    /*
    Returns true iff (row, col) describes a valid Square.
    */
    static bool isValidDims(std::size_t row, std::size_t col);

    /*
    Returns true iff `index` describes a valid square.
    */
    static bool isValidIndex(std::size_t index);

    /*
    Returns a square's index on [0, Square::NUM_SQUARES).

    The index-Square mapping given by this function is bijective and
    reversible by indexToSquare.
    */
    static SquareIndex squareToIndex(Square square);

    /*
    Returns the index that corresponds to a Square.

    The Square-index mapping given by this function is bijective
    and reversible by squareToIndex.
    */
    static Square indexToSquare(SquareIndex index);
};

// Note: need non-member to support Square map keys
bool operator==(Square lhs, Square rhs);

std::ostream& operator<<(std::ostream& out, Square move);

}  // namespace board

namespace std {

template <> struct hash<board::Square> {
    std::size_t operator()(const board::Square x) const;
};

std::string to_string(board::Square square);

}  // namespace std

#endif  // BOARD_SQUARE_H_
