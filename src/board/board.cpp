// Copyright 2021 Alex Theimer

#include "board/board.h"

#include <unordered_map>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <sstream>
#include <string>

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

/*
################################################################################
Note:
    Most public Board methods have a corresponding method suffixed by "Index".
    These methods contain the "guts" of all the public methods.

    The counterparts exist because (1) they call each other, (2) the public
    methods accept only Squares, and (3) lots of unnecessary Square -> index
    computation gets expensive quick.
################################################################################
*/

static const std::unordered_map<Piece, char> PIECE_CHAR_MAP = {
        { Piece{ PieceType::BISHOP, PieceColor::BLACK }, 'B' },
        { Piece{ PieceType::BISHOP, PieceColor::WHITE }, 'b' },
        { Piece{ PieceType::KNIGHT, PieceColor::BLACK }, 'N' },
        { Piece{ PieceType::KNIGHT, PieceColor::WHITE }, 'n' },
        { Piece{ PieceType::ROOK,  PieceColor::BLACK }, 'R' },
        { Piece{ PieceType::ROOK, PieceColor::WHITE }, 'r' },
        { Piece{ PieceType::KING, PieceColor::BLACK }, 'K' },
        { Piece{ PieceType::KING, PieceColor::WHITE }, 'k' },
        { Piece{ PieceType::QUEEN, PieceColor::BLACK }, 'Q' },
        { Piece{ PieceType::QUEEN, PieceColor::WHITE }, 'q' },
        { Piece{ PieceType::PAWN, PieceColor::BLACK }, 'P' },
        { Piece{ PieceType::PAWN, PieceColor::WHITE }, 'p' }
};

std::string makeIndexSquareString(std::size_t index) {
    ASSERT(Square::isValidIndex(index),
            "invalid index: " + std::to_string(index));
    std::stringstream ss;
    ss << "index: " << std::to_string(index)
       << ", square: " << Square::indexToSquare(index);
    return ss.str();
}

std::string Board::toString() const {
    std::stringstream ss;
    ss << "  0 1 2 3 4 5 6 7\n";
    for (std::size_t irow = 0; irow < Board::WIDTH; ++irow) {
        ss << std::to_string(irow) << ' ';
        for (std::size_t icol = 0; icol < Board::WIDTH; ++icol) {
            Square square(irow, icol);
            if (squareIsOccupied(square)) {
                Piece piece = getPiece(square);
                ss << PIECE_CHAR_MAP.at(piece) << ' ';
            } else {
                ss << "- ";
            }
        }
        ss << '\n';
    }
    return ss.str();
}

std::ostream& Board::operator<<(std::ostream& ostream) const {
    ostream << toString();
    return ostream;
}

bool Board::squareIsOccupiedIndex(std::size_t index) const {
    ASSERT(Square::isValidIndex(index),
            "invalid index: " + std::to_string(index));
    // get the bit at the union of the two color bitboards
    // TODO(theimer): keep a separate "all-piece" bitboard?
    Bitboard occupancy_board =
          (this->color_bitboards_[static_cast<std::size_t>(PieceColor::WHITE)] |
           this->color_bitboards_[static_cast<std::size_t>(PieceColor::BLACK)]);
    return static_cast<bool>(util::getBit(occupancy_board, index));
}

bool Board::squareIsOccupiedColorIndex(std::size_t index,
                                       PieceColor color) const {
    ASSERT(Square::isValidIndex(index),
            "invalid index: " + std::to_string(index));
    Bitboard color_board = color_bitboards_[static_cast<std::size_t>(color)];
    return util::getBit(color_board, index);
}

PieceColor Board::getPieceColorIndex(std::size_t index) const {
    ASSERT(Square::isValidIndex(index),
            "invalid index: " + std::to_string(index));
    ASSERT(squareIsOccupiedIndex(index),
            "unoccupied index: " + makeIndexSquareString(index));
    // step thru all PieceColor bitboards and check for occupancy at `square`
    for (std::size_t i = 0;
           i < static_cast<std::size_t>(PieceColor::NUM_PIECE_COLORS);
           ++i) {
        Bitboard board = color_bitboards_[i];
        if (util::getBit(board, index)) {
            return static_cast<PieceColor>(i);
        }
    }
    throw std::runtime_error("should never reach here");
}

// TODO(theimer): basically a copy-pasted of getPieceColoIndex
PieceType Board::getPieceTypeIndex(std::size_t index) const {
    ASSERT(Square::isValidIndex(index),
            "invalid index: " + std::to_string(index));
    ASSERT(squareIsOccupiedIndex(index),
            "unoccupied index: " + makeIndexSquareString(index));
    // step thru all PieceType bitboards and check for occupancy at the index
    for (std::size_t i = 0;
            i < static_cast<std::size_t>(PieceType::NUM_PIECE_TYPES);
            ++i) {
        Bitboard board = piece_bitboards_[i];
        if (util::getBit(board, index)) {
            return static_cast<PieceType>(i);
        }
    }
    throw std::runtime_error("should never reach here");
}

Piece Board::getPieceIndex(std::size_t index) const {
    ASSERT(Square::isValidIndex(index),
            "invalid index: " + std::to_string(index));
    PieceType type = getPieceTypeIndex(index);
    PieceColor color = getPieceColorIndex(index);
    return Piece{ type, color };
}

void Board::setPieceIndex(Piece piece, std::size_t index) {
    ASSERT(Square::isValidIndex(index),
            "invalid index: " + std::to_string(index));
    ASSERT(!squareIsOccupiedIndex(index),
            "index occupied: " + makeIndexSquareString(index));
    util::setBit(&this->piece_bitboards_[static_cast<std::size_t>(piece.type)],
                 index, true);
    util::setBit(&this->color_bitboards_[static_cast<std::size_t>(piece.color)],
                 index, true);
    // All Board constructors initialize hash_, so I have no idea why gcc
    //     thinks this might be uninitialized.
    hash_ = toggleZobPiece(hash_, piece, index);
    ASSERT(squareIsOccupiedIndex(index),
            "index unoccupied after set: " + makeIndexSquareString(index));
}

void Board::movePieceIndex(std::size_t from_index, std::size_t to_index) {
    ASSERT(Square::isValidIndex(from_index),
            "invalid 'from' index: " + std::to_string(from_index));
    ASSERT(Square::isValidIndex(to_index),
            "invalid 'to' index: " + std::to_string(to_index));
    ASSERT(squareIsOccupiedIndex(from_index),
            "from index unoccupied: " + makeIndexSquareString(from_index));
    ASSERT(!squareIsOccupiedIndex(to_index),
            "to index occupied: " + makeIndexSquareString(to_index));
    Piece from_piece = getPieceIndex(from_index);
    setPieceIndex(from_piece, to_index);
    removePieceIndex(from_index);

    // ifdef w/ named variable prevents different
    //    getPiece results in the assertions
    #ifdef DEBUG
    Piece to_piece = getPieceIndex(to_index);
    ASSERT(to_piece == from_piece,
            std::string("incorrect piece at 'to' after set\n")
            + "expected: " + std::to_string(from_piece)
            + ", actual: " + std::to_string(to_piece));
    ASSERT(!squareIsOccupiedIndex(from_index),
            "from index occupied after move: "
            + makeIndexSquareString(from_index));
    #endif  // DEBUG
}

void Board::removePieceIndex(std::size_t index) {
    ASSERT(Square::isValidIndex(index),
            "invalid index: " + std::to_string(index));

    // deal with the hash (TODO(theimer): something better!)
    if (squareIsOccupiedIndex(index)) {
        Piece piece = getPieceIndex(index);
        hash_ = toggleZobPiece(hash_, piece, index);
    }

    // set the color and piece bits to zero
    for (std::size_t i = 0;
            i < static_cast<std::size_t>(PieceColor::NUM_PIECE_COLORS);
            ++i) {
        util::setBit(&color_bitboards_[i], index, false);
    }
    for (std::size_t i = 0;
            i < static_cast<std::size_t>(PieceType::NUM_PIECE_TYPES);
            ++i) {
        util::setBit(&piece_bitboards_[i], index, false);
    }
    ASSERT(!squareIsOccupiedIndex(index),
            "index occupied after remove: " + makeIndexSquareString(index));
}

void Board::setPieceOverwriteIndex(Piece piece, std::size_t index) {
    ASSERT(Square::isValidIndex(index),
            "invalid index: " + std::to_string(index));
    // TODO(theimer): current impl is lazy and slow
    if (squareIsOccupiedIndex(index)) {
        removePieceIndex(index);
    }
    util::setBit(&this->piece_bitboards_[static_cast<std::size_t>(piece.type)],
                 index, true);
    util::setBit(&this->color_bitboards_[static_cast<std::size_t>(piece.color)],
                 index, true);


    hash_ = toggleZobPiece(hash_, piece, index);

    // ifdef w/ named variable prevents different
    //    getPiece results in the assertions
    #ifdef DEBUG
    Piece set_piece = getPieceIndex(index);
    ASSERT(set_piece == piece,
            std::string("incorrect piece at index after set\n")
            + "expected: " + std::to_string(piece)
            + ", actual: " + std::to_string(set_piece));
    #endif  // DEBUG
}

void Board::movePieceOverwriteIndex(std::size_t from_index,
                                    std::size_t to_index) {
    ASSERT(Square::isValidIndex(from_index),
            "invalid 'from' index: " + std::to_string(from_index));
    ASSERT(Square::isValidIndex(to_index),
            "invalid 'to' index: " + std::to_string(to_index));
    ASSERT(squareIsOccupiedIndex(from_index),
            "from index unoccupied: " + makeIndexSquareString(from_index));
    Piece from_piece = getPieceIndex(from_index);
    removePieceIndex(from_index);
    setPieceOverwriteIndex(from_piece, to_index);

    // ifdef w/ named variable prevents different
    //    getPiece results in the assertions
    #ifdef DEBUG
    Piece to_piece = getPieceIndex(to_index);
    ASSERT(to_piece == from_piece,
            std::string("incorrect piece at 'to' after set\n")
            + "expected: " + std::to_string(from_piece)
            + ", actual: " + std::to_string(to_piece));
    ASSERT(!squareIsOccupiedIndex(from_index),
            "from index occupied after move: "
            + makeIndexSquareString(from_index));
    #endif  // DEBUG
}

Board::Board() : hash_(board::ZOB_INIT) {
    // intentionally blank
}

Board::Board(const Board& other) : hash_(other.hash_) {
    // TODO(theimer): C++ memmove?
    for (std::size_t i = 0;
            i < static_cast<std::size_t>(PieceColor::NUM_PIECE_COLORS);
            ++i) {
        color_bitboards_[i] = other.color_bitboards_[i];
    }
    for (std::size_t i = 0;
            i < static_cast<std::size_t>(PieceType::NUM_PIECE_TYPES);
            ++i) {
        piece_bitboards_[i] = other.piece_bitboards_[i];
    }
}

Board::Board(const std::unordered_map<Square, Piece>& piece_map) {
    // Just step thru map elements and set each piece at its square.
    // Note: all field array indices are already initialized to zero.
    std::size_t hash = board::ZOB_INIT;
    for (auto iterator = piece_map.begin();
             iterator != piece_map.end();
             ++iterator)  {
        Square square = iterator->first;
        Piece piece = iterator->second;
        ASSERT(!this->squareIsOccupied(square),
                "square occupied: " + std::to_string(square));
        SquareIndex square_index = Square::squareToIndex(square);
        this->setPieceIndex(piece, square_index);
        hash = toggleZobPiece(hash, piece, square_index);
    }
    hash_ = hash;
}

bool Board::squareIsOccupied(Square square) const {
    std::size_t index = Square::squareToIndex(square);
    return squareIsOccupiedIndex(index);
}

bool Board::squareIsOccupiedColor(Square square, PieceColor color) const {
    std::size_t index = Square::squareToIndex(square);
    return squareIsOccupiedColorIndex(index, color);
}

void Board::setPiece(Piece piece, Square square) {
    std::size_t index = Square::squareToIndex(square);
    setPieceIndex(piece, index);
}

void Board::setPieceOverwrite(Piece piece, Square square) {
    std::size_t index = Square::squareToIndex(square);
    setPieceOverwriteIndex(piece, index);
}

PieceType Board::getPieceType(Square square) const {
    std::size_t index = Square::squareToIndex(square);
    return getPieceTypeIndex(index);
}

PieceColor Board::getPieceColor(Square square) const {
    std::size_t index = Square::squareToIndex(square);
    return getPieceColorIndex(index);
}

void Board::removePiece(Square square) {
    std::size_t index = Square::squareToIndex(square);
    removePieceIndex(index);
}

void Board::movePiece(Square from, Square to) {
    std::size_t from_index = Square::squareToIndex(from);
    std::size_t to_index = Square::squareToIndex(to);
    movePieceIndex(from_index, to_index);
}

void Board::movePieceOverwrite(Square from, Square to) {
    std::size_t from_index = Square::squareToIndex(from);
    std::size_t to_index = Square::squareToIndex(to);
    movePieceOverwriteIndex(from_index, to_index);
}

Piece Board::getPiece(Square square) const {
    std::size_t index = Square::squareToIndex(square);
    return getPieceIndex(index);
}

std::size_t std::hash<Board>::operator()(const board::Board& board) const {
    return board.hash_;
}

