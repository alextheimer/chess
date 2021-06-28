#ifndef BOARD_PIECE_H_
#define BOARD_PIECE_H_

#include <string>

#include "util/math.h"

namespace board {

enum class PieceType { KING, QUEEN, PAWN, ROOK, KNIGHT, BISHOP, NUM_PIECE_TYPES };
enum class PieceColor { BLACK, WHITE, NUM_PIECE_COLORS };

// TODO(theimer): move all piece compression stuff to a different namespace?
typedef uint8_t CompressedPiece;

typedef struct Piece {
    PieceType type;
    PieceColor color;
} Piece;

bool operator==(const Piece lhs, const Piece rhs);
std::ostream& operator<<(std::ostream& ostream, const Piece piece);

CompressedPiece compressPiece(const Piece piece);
Piece decompressPiece(CompressedPiece compressed_piece);

std::string toString(PieceColor color);
std::string toString(PieceType type);
std::string toString(const Piece piece);

}

#endif  // BOARD_PIECE_H_
