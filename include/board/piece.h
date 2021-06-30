// Copyright 2021 Alex Theimer

#ifndef BOARD_PIECE_H_
#define BOARD_PIECE_H_

#include <string>

#include "util/math.h"

namespace board {

enum class PieceType { KING, QUEEN, PAWN, ROOK, KNIGHT,
                       BISHOP, NUM_PIECE_TYPES };
enum class PieceColor { BLACK, WHITE, NUM_PIECE_COLORS };

// TODO(theimer): move all piece compression stuff to a different namespace?
typedef uint8_t CompressedPiece;

struct Piece {
    PieceType type;
    PieceColor color;
};

bool operator==(Piece lhs, Piece rhs);
std::ostream& operator<<(std::ostream& ostream, Piece piece);

PieceColor oppositeColor(PieceColor color);

CompressedPiece compressPiece(Piece piece);
Piece decompressPiece(CompressedPiece compressed_piece);

}  // namespace board

namespace std {

std::string to_string(board::PieceColor color);
std::string to_string(board::PieceType type);
std::string to_string(board::Piece piece);

}  // namespace std

#endif  // BOARD_PIECE_H_
