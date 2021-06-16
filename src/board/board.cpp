#include "board/board.h"

#include <cassert>
#include <unordered_map>
#include <algorithm>
#include <utility>

#include "util/bitops.h"
#include "util/math.h"

using namespace board;

typedef size_t BitboardIndex;

Square::Square(DimIndex row, DimIndex col) : row(row), col(col) {
    assert(row >= 0 && row < Board::WIDTH);
    assert(col >= 0 && col < Board::WIDTH);
}

bool board::operator==(const Square& lhs, const Square& rhs) {
    return (lhs.row == rhs.row) && (lhs.col == rhs.col);
}

size_t std::hash<Square>::operator()(const Square& x) const {
    hash<DimIndex> dim_hash;
    return dim_hash(x.row) ^ dim_hash(x.col);
}

BitboardIndex squareToBitboardIndex(const Square& square) {
    BitboardIndex index = (square.row * Board::WIDTH) + square.col;
    assert(index >= 0 && index < Board::SIZE);
    return index;
}

Square bitboardIndexToSquare(const BitboardIndex index) {
    assert(index >= 0 && index < Board::SIZE);
    return Square(index / Board::WIDTH, index % Board::WIDTH);
}

Board::Board(const std::unordered_map<Square, Piece>& piece_map) {
    // Note: all field array indices initialized to zero.
    for (auto iterator = piece_map.begin(); iterator != piece_map.end(); ++iterator)  {
        const Square& square = iterator->first;
        const Piece& piece = iterator->second;

        assert(!this->squareIsOccupied(square));

        this->setPiece(piece, square);
    }
}

bool Board::squareIsOccupied(const Square& square) const {
    size_t index = squareToBitboardIndex(square);
    Bitboard occupancy_board = (this->color_bitboards_[static_cast<int>(PieceColor::WHITE)] |
                               this->color_bitboards_[static_cast<int>(PieceColor::BLACK)]);
    return static_cast<bool>(bitops::getBit(occupancy_board, index));
}

void Board::setPiece(const Piece& piece, const Square& square) {
    assert(!this->squareIsOccupied(square));
    size_t index = squareToBitboardIndex(square);
    bitops::setBit(&this->piece_bitboards_[static_cast<int>(piece.type)], index, true);
    bitops::setBit(&this->color_bitboards_[static_cast<int>(piece.color)], index, true);
}
