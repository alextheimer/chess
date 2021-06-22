#ifndef BOARD_BOARD_H_
#define BOARD_BOARD_H_

#include <cstdint>
#include <ostream>
#include <unordered_map>

#include "board/piece.h"

namespace board {

// row/col Board index
typedef uint8_t DimIndex;

// TODO(theimer): move this somewhere else?
// "Backend" type used only in Board definition and implementation file.
// Stores a single bit of data for each of the 64 spaces on a Board.
typedef uint64_t Bitboard;

// TODO(theimer): make all structs immutable
class Square {
 public:

    // would make this POD, but want to enforce constructor initialization
    //     to support row/col assertions.

    DimIndex row;
    DimIndex col;

    /**
     * row and col must each lie on [0, Board::WIDTH).
     */
    Square(DimIndex row, DimIndex col);

    std::string toString();

};

// need non-member to support Square keys in maps
bool operator==(const Square& lhs, const Square& rhs);

struct Move {
    Square from;
    Square to;
    std::string toString();
};

bool operator==(const Move& lhs, const Move& rhs);

std::ostream& operator<<(std::ostream& out, Move& move);
std::ostream& operator<<(std::ostream& out, Square& move);

/**
 * Maintains an 8x8 chess board.
 */
class Board {
    /*
     * Each Board is maintained by a series of "Bitboards", where each Bitboard
     * stores one bit of data for each of the 64 Board spaces.
     *
     * A Bitboard is stored for each PieceColor and PieceType. TODO(theimer): expand.
     */

 private:
    Bitboard piece_bitboards_[static_cast<int>(PieceType::NUM_PIECE_TYPES)] = { 0 };
    Bitboard color_bitboards_[static_cast<int>(PieceColor::NUM_PIECE_COLORS)] = { 0 };

 public:
    static const std::size_t WIDTH = 8;
    static const std::size_t SIZE = WIDTH * WIDTH;

    Board();

    /**
     * Constructs a Board instance from a Square->Piece mapping.
     * I.e. for each pair (square, piece), piece is stored at square.
     *
     * TODO(theimer): expand.
     */
    explicit Board(const std::unordered_map<Square, Piece>& piece_map);

    /**
     * Returns true iff `square` is occupied on the Board.
     */
    bool squareIsOccupied(const Square& square) const;

    /**
     * Stores the piece described by `piece` at `square` on the Board.
     */
    void setPiece(const Piece& piece, const Square& square);

    // TODO(theimer): document
    void movePiece(Move& move);

    // TODO(theimer): document
    PieceType getPieceType(Square& square);

    // TODO(theimer): document
    PieceColor getPieceColor(Square& square);

    //TODO(theimer): document
    Piece getPiece(Square& square);

    // TODO(theimer): document
    void removePiece(Square& square);

    // TODO(theimer): document
    std::size_t getOccupiedSquares(PieceColor color, Square * buffer);

    // TODO(theimer): document
    std::size_t getOccupiedSquares(PieceType type, Square * buffer);

    // TODO(theimer): document
    std::size_t getOccupiedSquares(Square * buffer);

    // TODO(theimer): document
    bool squareIsOccupiedColor(const Square& square, PieceColor color);
};

}  // namespace board

namespace std {

template <> struct hash<board::Square> {
    size_t operator()(const board::Square& x) const;
};

template <> struct hash<board::Piece> {
    size_t operator()(const board::Piece& x) const;
};

}  // namespace std

#endif  // BOARD_BOARD_H_
