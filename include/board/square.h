#ifndef BOARD_SQUARE_H_
#define BOARD_SQUARE_H_

#include <cstdint>
#include <ostream>

namespace board {

// datatype used for Board row/col indices
typedef uint8_t DimIndex;

// TODO(theimer): make Square/Piece/Move immutable

struct Square {
    // this is really just a POD type, but I want to enforce
    //     constructor initialization to support row/col assertions.

    // TODO(theimer): explain why it's setup this way. (also isValidDims)
    static constexpr DimIndex MAX_DIM_VALUE = 8;

    DimIndex row;
    DimIndex col;

    /*
    row and col must each lie on [0, Board::WIDTH).
    */
    Square(DimIndex row, DimIndex col);

    /*
    Returns true iff (row, col) describes a valid Square.
    */
    static bool isValidDims(std::size_t row, std::size_t col);
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
