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

// TODO(theimer): make these constexpr
const std::size_t NUM_WIDTH_BITS = util::log2Ceil(Board::WIDTH);
const std::size_t WIDTH_MASK =
        (static_cast<std::size_t>(1) << NUM_WIDTH_BITS) - 1;

static const std::unordered_map<Piece, char> PIECE_CHAR_MAP = {
        { (Piece){ PieceType::BISHOP, PieceColor::BLACK }, 'B' },
        { (Piece){ PieceType::BISHOP, PieceColor::WHITE }, 'b' },
        { (Piece){ PieceType::KNIGHT, PieceColor::BLACK }, 'N' },
        { (Piece){ PieceType::KNIGHT, PieceColor::WHITE }, 'n' },
        { (Piece){ PieceType::ROOK,  PieceColor::BLACK }, 'R' },
        { (Piece){ PieceType::ROOK, PieceColor::WHITE }, 'r' },
        { (Piece){ PieceType::KING, PieceColor::BLACK }, 'K' },
        { (Piece){ PieceType::KING, PieceColor::WHITE }, 'k' },
        { (Piece){ PieceType::QUEEN, PieceColor::BLACK }, 'Q' },
        { (Piece){ PieceType::QUEEN, PieceColor::WHITE }, 'q' },
        { (Piece){ PieceType::PAWN, PieceColor::BLACK }, 'P' },
        { (Piece){ PieceType::PAWN, PieceColor::WHITE }, 'p' }
};

std::size_t squareToBitboardIndex(Square square) {
    std::size_t index = (static_cast<std::size_t>(square.row) << NUM_WIDTH_BITS)
            | static_cast<std::size_t>(square.col);
    ASSERT(index >= 0 && index < Board::SIZE,
            "index: " + std::to_string(index));
    return index;
}

Square bitboardIndexToSquare(std::size_t index) {
    ASSERT(index >= 0 && index < Board::SIZE,
            "index: " + std::to_string(index));
    DimIndex col = index & WIDTH_MASK;
    DimIndex row = index >> NUM_WIDTH_BITS;
    return Square(row, col);
}

bool isValidBitboardIndex(std::size_t index) {
    // >=0 just in case datatype changes
    //    (this should only exist in assertions, anyways)
    return (index < Board::SIZE) && (index >= 0);
}

std::string makeIndexSquareString(std::size_t index) {
    ASSERT(isValidBitboardIndex(index),
            "invalid index: " + std::to_string(index));
    std::stringstream ss;
    ss << "index: " << std::to_string(index)
       << ", square: " << bitboardIndexToSquare(index);
    return ss.str();
}

bool Square::isValidDims(std::size_t row, std::size_t col) {
    return (row < Board::WIDTH) && (col < Board::WIDTH);
}

std::string Board::toString() const {
    std::stringstream ss;
    ss << "  0 1 2 3 4 5 6 7\n";
    for (int irow = 0; irow < static_cast<int>(Board::WIDTH); ++irow) {
        ss << std::to_string(irow) << ' ';
        for (int icol = 0; icol < static_cast<int>(Board::WIDTH); ++icol) {
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
    ASSERT(isValidBitboardIndex(index),
            "invalid index: " + std::to_string(index));
    // get the bit at the union of the two color bitboards
    // TODO(theimer): keep a separate "all-piece" bitboard?
    Bitboard occupancy_board =
            (this->color_bitboards_[static_cast<int>(PieceColor::WHITE)] |
             this->color_bitboards_[static_cast<int>(PieceColor::BLACK)]);
    return static_cast<bool>(util::getBit(occupancy_board, index));
}

bool Board::squareIsOccupiedColorIndex(std::size_t index,
                                       PieceColor color) const {
    ASSERT(isValidBitboardIndex(index),
            "invalid index: " + std::to_string(index));
    Bitboard color_board = color_bitboards_[static_cast<std::size_t>(color)];
    return util::getBit(color_board, index);
}

PieceColor Board::getPieceColorIndex(std::size_t index) const {
    ASSERT(isValidBitboardIndex(index),
            "invalid index: " + std::to_string(index));
    ASSERT(squareIsOccupiedIndex(index),
            "unoccupied index: " + makeIndexSquareString(index));
    // step thru all PieceColor bitboards and check for occupancy at `square`
    for (int i = 0; i < static_cast<int>(PieceColor::NUM_PIECE_COLORS); ++i) {
        Bitboard board = color_bitboards_[i];
        if (util::getBit(board, index)) {
            return static_cast<PieceColor>(i);
        }
    }
    throw std::runtime_error("should never reach here");
}

// TODO(theimer): basically a copy-pasted of getPieceColoIndex
PieceType Board::getPieceTypeIndex(std::size_t index) const {
    ASSERT(isValidBitboardIndex(index),
            "invalid index: " + std::to_string(index));
    ASSERT(squareIsOccupiedIndex(index),
            "unoccupied index: " + makeIndexSquareString(index));
    // step thru all PieceType bitboards and check for occupancy at the index
    for (int i = 0; i < static_cast<int>(PieceType::NUM_PIECE_TYPES); ++i) {
        Bitboard board = piece_bitboards_[i];
        if (util::getBit(board, index)) {
            return static_cast<PieceType>(i);
        }
    }
    throw std::runtime_error("should never reach here");
}

Piece Board::getPieceIndex(std::size_t index) const {
    ASSERT(isValidBitboardIndex(index),
            "invalid index: " + std::to_string(index));
    PieceType type = getPieceTypeIndex(index);
    PieceColor color = getPieceColorIndex(index);
    return (Piece){ type, color };
}

void Board::setPieceIndex(Piece piece, std::size_t index) {
    ASSERT(isValidBitboardIndex(index),
            "invalid index: " + std::to_string(index));
    ASSERT(!squareIsOccupiedIndex(index),
            "index occupied: " + makeIndexSquareString(index));
    util::setBit(&this->piece_bitboards_[static_cast<int>(piece.type)],
                 index, true);
    util::setBit(&this->color_bitboards_[static_cast<int>(piece.color)],
                 index, true);
    ASSERT(squareIsOccupiedIndex(index),
            "index unoccupied after set: " + makeIndexSquareString(index));
}

void Board::movePieceIndex(std::size_t from_index, std::size_t to_index) {
    ASSERT(isValidBitboardIndex(from_index),
            "invalid 'from' index: " + std::to_string(from_index));
    ASSERT(isValidBitboardIndex(to_index),
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
    ASSERT(isValidBitboardIndex(index),
            "invalid index: " + std::to_string(index));
    // set the color and piece bits to zero
    for (int i = 0; i < static_cast<int>(PieceColor::NUM_PIECE_COLORS); ++i) {
        util::setBit(&color_bitboards_[i], index, false);
    }
    for (int i = 0; i < static_cast<int>(PieceType::NUM_PIECE_TYPES); ++i) {
        util::setBit(&piece_bitboards_[i], index, false);
    }
    ASSERT(!squareIsOccupiedIndex(index),
            "index occupied after remove: " + makeIndexSquareString(index));
}

void Board::setPieceOverwriteIndex(Piece piece, std::size_t index) {
    ASSERT(isValidBitboardIndex(index),
            "invalid index: " + std::to_string(index));
    // TODO(theimer): current impl is lazy and slow
    if (squareIsOccupiedIndex(index)) {
        removePieceIndex(index);
    }
    util::setBit(&this->piece_bitboards_[static_cast<int>(piece.type)],
                 index, true);
    util::setBit(&this->color_bitboards_[static_cast<int>(piece.color)],
                 index, true);

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
    ASSERT(isValidBitboardIndex(from_index),
            "invalid 'from' index: " + std::to_string(from_index));
    ASSERT(isValidBitboardIndex(to_index),
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
        std::size_t index = util::popHighestBit(&board);
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
    out << std::to_string(square);
    return out;
}

size_t std::hash<Square>::operator()(Square square) const {
    return squareToBitboardIndex(square);
}

std::string std::to_string(Square square) {
    std::stringstream sstr;
    sstr << "Square(row: " << std::to_string(square.row)
         << ", col: " << std::to_string(square.col) << ")";
    return sstr.str();
}

size_t std::hash<Piece>::operator()(const board::Piece piece) const {
    // TODO(theimer): use board::compressPiece?
    static constexpr std::size_t TYPE_SHIFT = 10;  // Arbitrary choice
    return (static_cast<size_t>(piece.type) << TYPE_SHIFT)
          | static_cast<size_t>(piece.color);
}

Board::Board() {
    // intentionally blank
}

Board::Board(const std::unordered_map<Square, Piece>& piece_map) {
    // Just step thru map elements and set each piece at its square.
    // Note: all field array indices are already initialized to zero.
    for (auto iterator = piece_map.begin();
             iterator != piece_map.end();
             ++iterator)  {
        Square square = iterator->first;
        Piece piece = iterator->second;
        ASSERT(!this->squareIsOccupied(square),
                "square occupied: " + std::to_string(square));
        this->setPiece(piece, square);
    }
}

bool Board::squareIsOccupied(Square square) const {
    std::size_t index = squareToBitboardIndex(square);
    return squareIsOccupiedIndex(index);
}

bool Board::squareIsOccupiedColor(Square square, PieceColor color) const {
    std::size_t index = squareToBitboardIndex(square);
    return squareIsOccupiedColorIndex(index, color);
}

void Board::setPiece(Piece piece, Square square) {
    std::size_t index = squareToBitboardIndex(square);
    setPieceIndex(piece, index);
}

void Board::setPieceOverwrite(Piece piece, Square square) {
    std::size_t index = squareToBitboardIndex(square);
    setPieceOverwriteIndex(piece, index);
}

PieceType Board::getPieceType(Square square) const {
    std::size_t index = squareToBitboardIndex(square);
    return getPieceTypeIndex(index);
}

PieceColor Board::getPieceColor(Square square) const {
    std::size_t index = squareToBitboardIndex(square);
    return getPieceColorIndex(index);
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
    std::size_t index = squareToBitboardIndex(square);
    removePieceIndex(index);
}

void Board::movePiece(Square from, Square to) {
    std::size_t from_index = squareToBitboardIndex(from);
    std::size_t to_index = squareToBitboardIndex(to);
    movePieceIndex(from_index, to_index);
}

void Board::movePieceOverwrite(Square from, Square to) {
    std::size_t from_index = squareToBitboardIndex(from);
    std::size_t to_index = squareToBitboardIndex(to);
    movePieceOverwriteIndex(from_index, to_index);
}

Piece Board::getPiece(Square square) const {
    std::size_t index = squareToBitboardIndex(square);
    return getPieceIndex(index);
}
