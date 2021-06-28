#include "board/board.h"

#include <unordered_map>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <sstream>

#include "util/assert.h"
#include "util/bitops.h"
#include "util/math.h"

using board::Board;
using board::Square;
using board::Piece;
using board::Bitboard;
using board::PieceType;
using board::PieceColor;
using board::DimIndex;

// TODO(theimer): get rid of this; just use std::size_t
typedef std::size_t BitboardIndex;

// TODO(theimer): make these constexpr
const std::size_t NUM_WIDTH_BITS = util::log2Ceil(Board::WIDTH);
const std::size_t WIDTH_MASK = (static_cast<std::size_t>(1) << NUM_WIDTH_BITS) - 1;

BitboardIndex squareToBitboardIndex(Square square) {
    BitboardIndex index = (static_cast<BitboardIndex>(square.row) << NUM_WIDTH_BITS)
            | static_cast<BitboardIndex>(square.col);
    ASSERT(index >= 0 && index < Board::SIZE, "index: " + std::to_string(index));
    return index;
}

Square bitboardIndexToSquare(BitboardIndex index) {
    ASSERT(index >= 0 && index < Board::SIZE, "index: " + std::to_string(index));
    DimIndex col = index & WIDTH_MASK;
    DimIndex row = index >> NUM_WIDTH_BITS;
    return Square(row, col);
}

bool getBitAtSquare(Bitboard board, Square square) {
    BitboardIndex index = squareToBitboardIndex(square);
    return util::getBit(board, index);
}

/*
Fills a buffer with all Squares such that each contains a 1 on the Bitboard.
@param buffer: an Iterator at the first index of the buffer.
@return: the number of Squares added to the buffer.
*/
std::size_t bitboardToSquares(Bitboard board, Square* buffer) {
     Square* ptr = buffer;
    // i.e. "while 1's exist on the board"
    // TODO(theimer): might vectorize with popcount-based for loop
    while (board > 0) {
        BitboardIndex index = util::popHighestBit(&board);
        *ptr = bitboardIndexToSquare(index);
        ++ptr;
    }
    return ptr - buffer;
}

Square::Square(DimIndex row, DimIndex col) : row(row), col(col) {
    ASSERT(row >= 0 && row < Board::WIDTH, "row: " + std::to_string(row));
    ASSERT(col >= 0 && col < Board::WIDTH, "col: " + std::to_string(col));
}

bool board::operator==(Square lhs, Square rhs) {
    return (lhs.row == rhs.row) && (lhs.col == rhs.col);
}

std::ostream& board::operator<<(std::ostream& out, Square square) {
    out << square.toString();
    return out;
}

size_t std::hash<Square>::operator()(Square square) const {
    return squareToBitboardIndex(square);
}

std::string Square::toString() const {
    std::stringstream sstr;
    sstr << "Square(row: " << std::to_string(row) << ", col: " << std::to_string(col) << ")";
    return sstr.str();
}

size_t std::hash<Piece>::operator()(const board::Piece piece) const {
    // TODO(theimer): use board::compressPiece?
    static constexpr std::size_t TYPE_SHIFT = 10;  // Arbitrary choice
    return (static_cast<size_t>(piece.type) << TYPE_SHIFT) | static_cast<size_t>(piece.color);
}

Board::Board() {
    // intentionally blank
}

Board::Board(const std::unordered_map<Square, Piece>& piece_map) {
    // Just step thru map elements and set each piece at its square.
    // Note: all field array indices are already initialized to zero.
    for (auto iterator = piece_map.begin(); iterator != piece_map.end(); ++iterator)  {
        Square square = iterator->first;
        Piece piece = iterator->second;
        ASSERT(!this->squareIsOccupied(square), "square occupied: " + square.toString());
        this->setPiece(piece, square);
    }
}

bool Board::squareIsOccupied(Square square) const {
    // get the bit at the union of the two color bitboards
    // TODO(theimer): keep a separate "all-piece" bitboard?
    std::size_t index = squareToBitboardIndex(square);
    Bitboard occupancy_board = (this->color_bitboards_[static_cast<int>(PieceColor::WHITE)] |
                               this->color_bitboards_[static_cast<int>(PieceColor::BLACK)]);
    return static_cast<bool>(util::getBit(occupancy_board, index));
}

void Board::setPiece(Piece piece, Square square) {
    // TODO(theimer): assert square is unoccupied if/when "overwrite" variant is made
    std::size_t index = squareToBitboardIndex(square);
    util::setBit(&this->piece_bitboards_[static_cast<int>(piece.type)], index, true);
    util::setBit(&this->color_bitboards_[static_cast<int>(piece.color)], index, true);
}

bool Board::squareIsOccupiedColor(Square square, PieceColor color) const {
    Bitboard color_board = color_bitboards_[static_cast<std::size_t>(color)];
    return getBitAtSquare(color_board, square);
}

PieceType Board::getPieceType(Square square) const {
    // step thru all PieceType bitboards and check for occupancy at `square`
    for (int i = 0; i < static_cast<int>(PieceType::NUM_PIECE_TYPES); ++i) {
        Bitboard board = piece_bitboards_[i];
        // TODO(theimer): lots of square -> index recalculation!
        if (getBitAtSquare(board, square)) {
            return static_cast<PieceType>(i);
        }
    }
    throw std::invalid_argument("unoccupied square: " + square.toString());
}

// TODO(theimer): basically copy-paste of the above
PieceColor Board::getPieceColor(Square square) const {
    // step thru all PieceColor bitboards and check for occupancy at `square`
    for (int i = 0; i < static_cast<int>(PieceColor::NUM_PIECE_COLORS); ++i) {
        Bitboard board = color_bitboards_[i];
        // TODO(theimer): lots of square -> index recalculation!
        if (getBitAtSquare(board, square)) {
            return static_cast<PieceColor>(i);
        }
    }
    throw std::invalid_argument("unoccupied square: " + square.toString());
}

std::size_t Board::getOccupiedSquares(PieceColor color, Square* buffer) const {
    Bitboard board = color_bitboards_[static_cast<size_t>(color)];
    return bitboardToSquares(board, buffer);
}


std::size_t Board::getOccupiedSquares(PieceType type, Square* buffer) const {
    Bitboard board = piece_bitboards_[static_cast<size_t>(type)];
    return bitboardToSquares(board, buffer);
}

std::size_t Board::getOccupiedSquares(Square* buffer) const {
    // get the union of all color bitboards, then convert to squares.
    Bitboard board = 0;
    for (int i = 0; i < static_cast<int>(PieceColor::NUM_PIECE_COLORS); ++i) {
        board |= color_bitboards_[i];
    }
    return bitboardToSquares(board, buffer);
}

void Board::removePiece(Square square) {
    // set the color and piece bits to zero
    BitboardIndex index = squareToBitboardIndex(square);
    for (int i = 0; i < static_cast<int>(PieceColor::NUM_PIECE_COLORS); ++i) {
        util::setBit(&color_bitboards_[i], index, false);
    }
    for (int i = 0; i < static_cast<int>(PieceType::NUM_PIECE_TYPES); ++i) {
        util::setBit(&piece_bitboards_[i], index, false);
    }
}

void Board::movePiece(Square from, Square to) {
    ASSERT(squareIsOccupied(from), "from: " + from.toString());
    // TODO(theimer): square -> index recomputation!
    Piece from_piece = getPiece(from);
    setPiece(from_piece, to);
    removePiece(from);
}

Piece Board::getPiece(Square square) const {
    PieceType type = getPieceType(square);
    PieceColor color = getPieceColor(square);
    return (Piece){ type, color };
}
