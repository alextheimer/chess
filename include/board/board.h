#ifndef BOARD_H_
#define BOARD_H_

#include <cstdint>
#include <unordered_map>

namespace board {

    static const int BOARD_WIDTH = 8;

    enum class PieceType { KING, PAWN, ROOK, KNIGHT, BISHOP, NUM_PIECE_TYPES };
    enum class PieceColor { BLACK, WHITE, NUM_PIECE_COLORS };

    struct Piece {
        const PieceType type;
        const PieceColor color;
        Piece(PieceType type, PieceColor color);
    };

    struct Square {
        const uint8_t row;
        const uint8_t col;
        Square(uint8_t row, uint8_t col);
    };

    class Board {
        private:
            uint64_t piece_bitboards_[static_cast<int>(PieceType::NUM_PIECE_TYPES)] = { 0 };
            uint64_t color_bitboards_[static_cast<int>(PieceColor::NUM_PIECE_COLORS)] = { 0 };
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
