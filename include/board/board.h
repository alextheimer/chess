#ifndef BOARD_H_
#define BOARD_H_

#include <cstdint>
#include <unordered_map>

namespace board {

enum class PieceType { KING, QUEEN, PAWN, ROOK, KNIGHT, BISHOP, NUM_PIECE_TYPES };
enum class PieceColor { BLACK, WHITE, NUM_PIECE_COLORS };

// row/col Board index
typedef uint8_t DimIndex;

// TODO(theimer): move this somewhere else?
// "Backend" type used only in Board definition and implementation file.
// Stores a single bit of data for each of the 64 spaces on a Board.
typedef uint64_t Bitboard;

typedef struct Piece {
    const PieceType type;
    const PieceColor color;
} Piece;

bool operator==(const Piece& lhs, const Piece& rhs);

class Square {
 public:

    // would make this POD, but want to enforce constructor initialization
    //     to support row/col assertions.

    const DimIndex row;
    const DimIndex col;

    /**
     * row and col must each lie on [0, Board::WIDTH).
     */
    Square(DimIndex row, DimIndex col);

};

// need non-member to support Square keys in maps
bool operator==(const Square& lhs, const Square& rhs);

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
};

}  // namespace board

// In the header file to support Board's map constructor.
namespace std {

template <> struct hash<board::Square> {
    size_t operator()(const board::Square& x) const;
};

}  // namespace std

#endif  // BOARD_H_
