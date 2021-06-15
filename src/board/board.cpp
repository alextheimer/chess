#include "board/board.h"

#include <cassert>
#include <unordered_map>
#include <algorithm>
#include <utility>

#include "util/bitops.h"
#include "util/math.h"

using namespace board;

typedef uint8_t CompressedPiece;
typedef size_t BitboardIndex;

const std::size_t NUM_PIECE_COLOR_BITS =
        math::log2Ceil(1 + static_cast<std::size_t>(PieceColor::NUM_PIECE_COLORS));
const std::size_t NUM_PIECE_TYPE_BITS =
        math::log2Ceil(1 + static_cast<std::size_t>(PieceType::NUM_PIECE_TYPES));
const std::size_t PIECE_COLOR_MASK =
        ((std::size_t)1 << NUM_PIECE_COLOR_BITS) - 1;
const std::size_t PIECE_TYPE_MASK =
        ((std::size_t)1 << NUM_PIECE_TYPE_BITS) - 1;

Square::Square(DimIndex row, DimIndex col) : row(row), col(col) {
    assert(row >= 0 && row < Board::WIDTH);
    assert(col >= 0 && col < Board::WIDTH);
}

bool board::operator==(const Piece& lhs, const Piece& rhs) {
    return (lhs.type == rhs.type) && (lhs.color == rhs.color);
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

CompressedPiece compressPiece(const Piece& piece) {
    CompressedPiece compressed_piece = static_cast<CompressedPiece>(piece.color);
    compressed_piece <<= NUM_PIECE_TYPE_BITS;
    compressed_piece |= static_cast<CompressedPiece>(piece.type);
    return compressed_piece;
}

Piece decompressPiece(CompressedPiece compressed_piece) {
    PieceType type;
    PieceColor color;

    type = static_cast<PieceType>(compressed_piece & PIECE_TYPE_MASK);
    compressed_piece >>= NUM_PIECE_TYPE_BITS;
    color = static_cast<PieceColor>(compressed_piece & PIECE_COLOR_MASK);

    // TODO(theimer): this seems too C-ish
    return (Piece){type, color};
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
