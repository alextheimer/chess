#ifndef BOARD_H_
#define BOARD_H_

#include <cstdint>
#include <unordered_map>

namespace board {

    const std::size_t BOARD_WIDTH = 8;
    const std::size_t BOARD_SIZE = BOARD_WIDTH * BOARD_WIDTH;

    enum class PieceType { KING, PAWN, ROOK, KNIGHT, BISHOP, NUM_PIECE_TYPES };
    enum class PieceColor { BLACK, WHITE, NUM_PIECE_COLORS };

    typedef uint8_t DimIndex;

    struct Piece {
        const PieceType type;
        const PieceColor color;
        Piece(PieceType type, PieceColor color);
    };

    struct Square {
        const DimIndex row;
        const DimIndex col;
        Square(DimIndex row, DimIndex col);
    };

    class Board {
        private:
            std::array<Bitboard, static_cast<int>(PieceType::NUM_PIECE_TYPES)> piece_bitboards_ = { 0 };
            std::array<Bitboard, static_cast<int>(PieceColor::NUM_PIECE_COLORS)> color_bitboards_ = { 0 };
        public:
            Board(const std::unordered_map<Square, Piece>& piece_map);
            Board(const Board& copy_me);
            Board(const Board&& move_me);
            ~Board();
            Board& operator=(const Board& copy_assign_me);
            Board& operator=(const Board&& move_assign_me);

            bool squareIsOccupied(const Square& square) const;
            void setPiece(const Piece& piece, const Square& square);
            Piece getPiece(const Square& square);
    };
}

#endif  // BOARD_H_
