// Copyright 2021 Alex Theimer

#include "board/square.h"

#include <string>
#include <sstream>

#include "util/assert.h"
#include "util/math.h"

using board::Square;
using board::SquareIndex;

// TODO(theimer): make these constexpr
const std::size_t NUM_DIM_BITS = util::log2Ceil(Square::MAX_DIM_VALUE);
const std::size_t DIM_MASK =
        (static_cast<std::size_t>(1) << NUM_DIM_BITS) - 1;

Square::Square(DimIndex row, DimIndex col) : row(row), col(col) {
    ASSERT(row >= 0 && row < Square::MAX_DIM_VALUE,
            "row: " + std::to_string(row));
    ASSERT(col >= 0 && col < Square::MAX_DIM_VALUE,
            "col: " + std::to_string(col));
}

bool Square::isValidDims(std::size_t row, std::size_t col) {
    return (row < Square::MAX_DIM_VALUE) && (col < Square::MAX_DIM_VALUE);
}

bool Square::isValidIndex(std::size_t index) {
    // >=0 just in case datatype changes
    //    (this should only exist in assertions, anyways)
    return (index < Square::NUM_SQUARES) && (index >= 0);
}

SquareIndex Square::squareToIndex(Square square) {
    std::size_t index = (static_cast<std::size_t>(square.row) << NUM_DIM_BITS)
            | static_cast<std::size_t>(square.col);
    ASSERT(index >= 0 && index < Square::NUM_SQUARES,
            "index: " + std::to_string(index));
    return index;
}

Square Square::indexToSquare(SquareIndex index) {
    ASSERT(index >= 0 && index < Square::NUM_SQUARES,
            "index: " + std::to_string(index));
    DimIndex col = index & DIM_MASK;
    DimIndex row = index >> NUM_DIM_BITS;
    return Square(row, col);
}

bool board::operator==(Square lhs, Square rhs) {
    return (lhs.row == rhs.row) && (lhs.col == rhs.col);
}

std::ostream& board::operator<<(std::ostream& out, Square square) {
    out << std::to_string(square);
    return out;
}

std::size_t std::hash<Square>::operator()(Square square) const {
    return Square::squareToIndex(square);
}

std::string std::to_string(Square square) {
    std::stringstream sstr;
    sstr << "Square(row: " << std::to_string(square.row)
         << ", col: " << std::to_string(square.col) << ")";
    return sstr.str();
}
