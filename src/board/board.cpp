#include "board/board.h"

#include <unordered_map>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <sstream>

#include "util/assert.h"
#include "util/bitops.h"
#include "util/math.h"

using namespace board;

typedef size_t BitboardIndex;

Board::Board() {
    // intentionally blank
}

Square::Square(DimIndex row, DimIndex col) : row(row), col(col) {
    ASSERT(row >= 0 && row < Board::WIDTH, "row: " + std::to_string(row));
    ASSERT(col >= 0 && col < Board::WIDTH, "col: " + std::to_string(col));
}

bool board::operator==(const Square& lhs, const Square& rhs) {
    return (lhs.row == rhs.row) && (lhs.col == rhs.col);
}

std::ostream& board::operator<<(std::ostream& out, const Move& move) {
    out << move.toString();
    return out;
}

std::ostream& board::operator<<(std::ostream& out, const Square& square) {
    out << square.toString();
    return out;
}

bool board::operator==(const Move& lhs, const Move& rhs) {
    return (lhs.from == rhs.from) && (lhs.to == rhs.to);
}

size_t std::hash<Square>::operator()(const Square& x) const {
    static const size_t ROW_SHIFT = 10;
    return (static_cast<size_t>(x.row) << ROW_SHIFT) | static_cast<size_t>(x.col);
}

size_t std::hash<Piece>::operator()(const board::Piece& x) const {
    static const size_t TYPE_SHIFT = 10;
    return (static_cast<size_t>(x.type) << TYPE_SHIFT) | static_cast<size_t>(x.color);
}

BitboardIndex squareToBitboardIndex(const Square& square) {
    BitboardIndex index = (square.row * Board::WIDTH) + square.col;
    ASSERT(index >= 0 && index < Board::SIZE, "index: " + std::to_string(index));
    return index;
}

Square bitboardIndexToSquare(const BitboardIndex index) {
    // TODO(theimer): just bit-shifts
    ASSERT(index >= 0 && index < Board::SIZE, "index: " + std::to_string(index));
    return Square(index / Board::WIDTH, index % Board::WIDTH);
}

bool getBitAtSquare(const Bitboard board, Square square) {
    BitboardIndex index = squareToBitboardIndex(square);
    return bitops::getBit(board, index);
}

Board::Board(const std::unordered_map<Square, Piece>& piece_map) {
    // Note: all field array indices initialized to zero.
    for (auto iterator = piece_map.begin(); iterator != piece_map.end(); ++iterator)  {
        const Square& square = iterator->first;
        const Piece& piece = iterator->second;

        ASSERT(!this->squareIsOccupied(square), "square: " + square.toString());

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
    ASSERT(!this->squareIsOccupied(square), "square: " + square.toString());
    size_t index = squareToBitboardIndex(square);
    bitops::setBit(&this->piece_bitboards_[static_cast<int>(piece.type)], index, true);
    bitops::setBit(&this->color_bitboards_[static_cast<int>(piece.color)], index, true);
}

bool Board::squareIsOccupiedColor(const Square& square, PieceColor color) {
    Bitboard color_board = color_bitboards_[static_cast<std::size_t>(color)];
    return getBitAtSquare(color_board, square);
}

PieceType Board::getPieceType(Square& square) {
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
PieceColor Board::getPieceColor(Square& square) {
    for (int i = 0; i < static_cast<int>(PieceColor::NUM_PIECE_COLORS); ++i) {
        Bitboard board = color_bitboards_[i];
        // TODO(theimer): lots of square -> index recalculation!
        if (getBitAtSquare(board, square)) {
            return static_cast<PieceColor>(i);
        }
    }
    throw std::invalid_argument("unoccupied square: " + square.toString());
}

std::size_t bitboardToSquares(Bitboard board, Square* buffer) {
     Square * ptr = buffer;
    // TODO(theimer): might vectorize with popcount
    while (board > 0) {
        BitboardIndex index = bitops::popHighestBit(&board);
        *ptr = bitboardIndexToSquare(index);
        ++ptr;
    }
    return ptr - buffer;
}

std::size_t Board::getOccupiedSquares(PieceColor color, Square * buffer) {
    Bitboard board = color_bitboards_[static_cast<size_t>(color)];
    return bitboardToSquares(board, buffer);
}


std::size_t Board::getOccupiedSquares(PieceType type, Square * buffer) {
    Bitboard board = piece_bitboards_[static_cast<size_t>(type)];
    return bitboardToSquares(board, buffer);
}

// TODO(theimer): worth maintaining this?
std::size_t Board::getOccupiedSquares(Square * buffer) {
    Bitboard board = 0;
    for (int i = 0; i < static_cast<int>(PieceColor::NUM_PIECE_COLORS); ++i) {
        board |= color_bitboards_[i];
    }
    return bitboardToSquares(board, buffer);
}

void Board::removePiece(Square& square) {
    BitboardIndex index = squareToBitboardIndex(square);
    for (int i = 0; i < static_cast<int>(PieceColor::NUM_PIECE_COLORS); ++i) {
        bitops::setBit(&color_bitboards_[i], index, false);
    }
    for (int i = 0; i < static_cast<int>(PieceType::NUM_PIECE_TYPES); ++i) {
        bitops::setBit(&piece_bitboards_[i], index, false);
    }
}

void Board::movePiece(Move& move) {
    ASSERT(squareIsOccupied(move.from), "from: " + move.from.toString());
    // TODO(theimer): square -> index recomputation!
    Piece from_piece = getPiece(move.from);
    setPiece(from_piece, move.to);
    removePiece(move.from);
}

Piece Board::getPiece(Square& square) {
    PieceType type = getPieceType(square);
    PieceColor color = getPieceColor(square);
    return (Piece){ type, color };
}

std::string Square::toString() const {
    std::stringstream sstr;
    sstr << "Square(row: " << row << ", col: " << col << ")";
    return sstr.str();
}

std::string Move::toString() const {
    std::stringstream sstr;
    sstr << "Move(from: " << from << ", to: " << to << ")";
    return sstr.str();
}
