#include "board/piece.h"

#include <cstdint>
#include <string>
#include <stdexcept>
#include <ostream>
#include <sstream>

using namespace board;

const std::size_t NUM_PIECE_COLOR_BITS =
        math::log2Ceil(1 + static_cast<std::size_t>(PieceColor::NUM_PIECE_COLORS));
const std::size_t NUM_PIECE_TYPE_BITS =
        math::log2Ceil(1 + static_cast<std::size_t>(PieceType::NUM_PIECE_TYPES));
const std::size_t PIECE_COLOR_MASK =
        ((std::size_t)1 << NUM_PIECE_COLOR_BITS) - 1;
const std::size_t PIECE_TYPE_MASK =
        ((std::size_t)1 << NUM_PIECE_TYPE_BITS) - 1;

std::string board::toString(const Piece& piece) {
    std::stringstream ss;
    ss << "Piece(type: " << toString(piece.type) << ", "
       << "color: " << toString(piece.color);
    return ss.str();
}

std::ostream& board::operator<<(std::ostream& ostream, const Piece& piece) {
    ostream << toString(piece);
    return ostream;
}

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

std::string board::toString(PieceColor color) {
    switch (color) {
    case PieceColor::BLACK: return "BLACK";
    case PieceColor::WHITE: return "WHITE";
    default: throw std::invalid_argument("unhandled PieceColor value: " + static_cast<int>(color));
    }
}

std::string board::toString(PieceType type) {
    switch (type) {
    case PieceType::BISHOP: return "BISHOP";
    case PieceType::ROOK: return "ROOK";
    case PieceType::KNIGHT: return "KNIGHT";
    case PieceType::PAWN: return "PAWN";
    case PieceType::QUEEN: return "QUEEN";
    case PieceType::KING: return "KING";
    default: throw std::invalid_argument("unhandled PieceType value: " + static_cast<int>(type));
    }
}
