#include "board/piece.h"

#include <cstdint>

using namespace board;

const std::size_t NUM_PIECE_COLOR_BITS =
        math::log2Ceil(1 + static_cast<std::size_t>(PieceColor::NUM_PIECE_COLORS));
const std::size_t NUM_PIECE_TYPE_BITS =
        math::log2Ceil(1 + static_cast<std::size_t>(PieceType::NUM_PIECE_TYPES));
const std::size_t PIECE_COLOR_MASK =
        ((std::size_t)1 << NUM_PIECE_COLOR_BITS) - 1;
const std::size_t PIECE_TYPE_MASK =
        ((std::size_t)1 << NUM_PIECE_TYPE_BITS) - 1;

CompressedPiece board::compressPiece(const Piece& piece) {
    CompressedPiece compressed_piece = static_cast<CompressedPiece>(piece.color);
    compressed_piece <<= NUM_PIECE_TYPE_BITS;
    compressed_piece |= static_cast<CompressedPiece>(piece.type);
    return compressed_piece;
}

Piece board::decompressPiece(CompressedPiece compressed_piece) {
    PieceType type;
    PieceColor color;

    type = static_cast<PieceType>(compressed_piece & PIECE_TYPE_MASK);
    compressed_piece >>= NUM_PIECE_TYPE_BITS;
    color = static_cast<PieceColor>(compressed_piece & PIECE_COLOR_MASK);

    // TODO(theimer): this seems too C-ish
    return (Piece){type, color};
}

bool board::operator==(const Piece& lhs, const Piece& rhs) {
    return (lhs.type == rhs.type) && (lhs.color == rhs.color);
}
