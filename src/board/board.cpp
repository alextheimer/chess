#include "board/board.h"

#include <cassert>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <stdexcept>

#include "util/bitops.h"
#include "util/math.h"

using namespace board;

typedef size_t BitboardIndex;

Board::Board() {
    // intentionally blank
}

Square::Square(DimIndex row, DimIndex col) : row(row), col(col) {
    assert(row >= 0 && row < Board::WIDTH);
    assert(col >= 0 && col < Board::WIDTH);
}

bool board::operator==(const Square& lhs, const Square& rhs) {
    return (lhs.row == rhs.row) && (lhs.col == rhs.col);
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
    assert(index >= 0 && index < Board::SIZE);
    return index;
}

Square bitboardIndexToSquare(const BitboardIndex index) {
    // TODO(theimer): just bit-shifts
    assert(index >= 0 && index < Board::SIZE);
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
    // TODO(theimer): make this work
    throw std::invalid_argument("unoccupied square: TODO");
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
    // TODO(theimer): make this work
    throw std::invalid_argument("unoccupied square: TODO");
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
    assert(squareIsOccupied(move.from));
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
