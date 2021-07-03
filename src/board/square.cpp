#include <sstream>

#include "board/square.h"
#include "util/assert.h"

using board::Square;

Square::Square(DimIndex row, DimIndex col) : row(row), col(col) {
    ASSERT(row >= 0 && row < Board::WIDTH, "row: " + std::to_string(row));
    ASSERT(col >= 0 && col < Board::WIDTH, "col: " + std::to_string(col));
}

bool Square::isValidDims(std::size_t row, std::size_t col) {
    return (row < Square::MAX_DIM_VALUE) && (col < Square::MAX_DIM_VALUE);
}

bool board::operator==(Square lhs, Square rhs) {
    return (lhs.row == rhs.row) && (lhs.col == rhs.col);
}

std::ostream& board::operator<<(std::ostream& out, Square square) {
    out << std::to_string(square);
    return out;
}

std::size_t std::hash<Square>::operator()(Square square) const {
    // TODO(theimer): make this concrete
    return (square.row << 10) | square.col;
}

std::string std::to_string(Square square) {
    std::stringstream sstr;
    sstr << "Square(row: " << std::to_string(square.row)
         << ", col: " << std::to_string(square.col) << ")";
    return sstr.str();
}
