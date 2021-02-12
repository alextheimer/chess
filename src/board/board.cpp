#include "util/bitops.h"
#include "board/board.h"

#include <cassert>
#include <unordered_map>
#include <algorithm>

#include "util/math.h"

using namespace board;

typedef uint8_t CompressedPiece;
const size_t NUM_PIECE_COLOR_BITS = math::log2Ceil(static_cast<int>(PieceColor::NUM_PIECE_COLORS));
const size_t NUM_PIECE_TYPE_BITS = math::log2Ceil(static_cast<int>(PieceType::NUM_PIECE_TYPES));
const size_t PIECE_COLOR_MASK = ((size_t)1 << NUM_PIECE_COLOR_BITS) - 1;
const size_t PIECE_TYPE_MASK = ((size_t)1 << NUM_PIECE_TYPE_BITS) - 1;

typedef uint8_t BitboardIndex;

Piece::Piece(PieceType type, PieceColor color) : type(type), color(color) {
    assert(type != PieceType::NUM_PIECE_TYPES);
    assert(color != PieceColor::NUM_PIECE_COLORS);
}

Square::Square(DimIndex row, DimIndex col) : row(row), col(col) {
    assert(row >= 0 && row < BOARD_WIDTH);
    assert(col >= 0 && col < BOARD_WIDTH);
}

BitboardIndex squareToBitboardIndex(const Square& square) {
    BitboardIndex index = (square.row * BOARD_WIDTH) + square.col;
    assert(index >= 0 && index < board::BOARD_SIZE);
    return index;
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

void board::swapBoard(Board& board1, Board& board2) {
	swap(board1.color_bitboards_, board2.color_bitboards_);
	swap(board1.piece_bitboards_, board2.piece_bitboards_);
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

Board::Board(const Board& copy_me) {
	std::copy(copy_me.color_bitboards_.begin(),
			  copy_me.color_bitboards_.end(),
			  this->color_bitboards_.begin());
	std::copy(copy_me.piece_bitboards_.begin(),
			  copy_me.piece_bitboards_.end(),
			  this->piece_bitboards_.begin());
}

Board::Board(Board&& move_me) {
	board::swapBoard(*this, move_me);
}

Board::~Board() {
	// blank
}

Board& Board::operator=(const Board& copy_assign_me) {
	Board copy(copy_assign_me);
	board::swapBoard(*this, copy);
	return *this;
}

Board& Board::operator=(Board&& move_assign_me) {
	board::swapBoard(*this, move_assign_me);
	return *this;
}

bool Board::squareIsOccupied(const Square& square) const {
    size_t index = squareToBitboardIndex(square);
    Bitboard occupancy_board = (this->color_bitboards_[static_cast<int>(PieceColor::WHITE)] |
                               this->color_bitboards_[static_cast<int>(PieceColor::BLACK)]);
    return static_cast<bool>(bitops::get_bit(occupancy_board, index));
}

void Board::setPiece(const Piece& piece, const Square& square) {
    assert(!this->squareIsOccupied(square));
    size_t index = squareToBitboardIndex(square);
    bitops::set_bit(this->piece_bitboards_[static_cast<int>(piece.type)], index, 1);
    bitops::set_bit(this->color_bitboards_[static_cast<int>(piece.color)], index, 1);
}
