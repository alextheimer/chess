#include "board/piece.h"

#include <cstdint>
#include <string>
#include <stdexcept>
#include <ostream>
#include <sstream>

using board::PieceType;
using board::PieceColor;
using board::Piece;
using board::CompressedPiece;

/*
Note: compressed pieces are built as follows:
|-- color --|-- type --|
*/

// Number of bits used to represent each enum in a compressed Piece.
// TODO(theimer): make all of these constexpr
static const std::size_t NUM_PIECE_COLOR_BITS =
        util::log2Ceil(static_cast<std::size_t>(PieceColor::NUM_PIECE_COLORS));
static const std::size_t NUM_PIECE_TYPE_BITS =
        util::log2Ceil(static_cast<std::size_t>(PieceType::NUM_PIECE_TYPES));

// masks used to extract Piece data from its compressed format
static const std::size_t PIECE_COLOR_MASK =
        ((std::size_t)1 << NUM_PIECE_COLOR_BITS) - 1;
static const std::size_t PIECE_TYPE_MASK =
        ((std::size_t)1 << NUM_PIECE_TYPE_BITS) - 1;

std::string board::toString(Piece piece) {
    std::stringstream ss;
    ss << "Piece(type: " << toString(piece.type) << ", "
       << "color: " << toString(piece.color);
    return ss.str();
}

CompressedPiece board::compressPiece(Piece piece) {
    CompressedPiece compressed_piece = static_cast<CompressedPiece>(piece.color);
    compressed_piece <<= NUM_PIECE_TYPE_BITS;
    compressed_piece |= static_cast<CompressedPiece>(piece.type);
    return compressed_piece;
}

Piece board::decompressPiece(CompressedPiece compressed_piece) {
    // extract from the least significant bits...
    PieceType type = static_cast<PieceType>(compressed_piece & PIECE_TYPE_MASK);
    compressed_piece >>= NUM_PIECE_TYPE_BITS;
    PieceColor color = static_cast<PieceColor>(compressed_piece & PIECE_COLOR_MASK);
    // TODO(theimer): this syntax seems too C-ish
    return (Piece){type, color};
}

bool board::operator==(Piece lhs, Piece rhs) {
    return (lhs.type == rhs.type) && (lhs.color == rhs.color);
}

std::ostream& board::operator<<(std::ostream& ostream, Piece piece) {
    ostream << toString(piece);
    return ostream;
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
