#ifndef BOARD_H_
#define BOARD_H_

#include <cstdint>
#include <unordered_map>

namespace board {

    enum class PieceType { PAWN, KING, NUM_PIECE_TYPES };
    enum class PieceColor { BLACK, WHITE, NUM_PIECE_COLORS };

    // row/col Board indices
    typedef uint8_t DimIndex;
    // Backend" type used only in Board definition and implementation file.
    // Stores a single bit of data for each of the 64 spaces on a Board.
    typedef uint64_t Bitboard;

    struct Piece {
        const PieceType type;
        const PieceColor color;
        /**
         * type and color cannot be a size-indicating enum value
         * (i.e. any with the prefix "NUM_").
         */
        Piece(PieceType type, PieceColor color);
    	friend bool operator==(const Piece& lhs, const Piece& rhs);
    };

    struct Square {
        const DimIndex row;
        const DimIndex col;
        /**
         * row and col must each lie on [0, Board::WIDTH).
         */
        Square(DimIndex row, DimIndex col);
        friend bool operator==(const Square& lhs, const Square& rhs);
    };

    bool operator==(const Piece& lhs, const Piece& rhs);
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
            std::array<Bitboard, static_cast<int>(PieceType::NUM_PIECE_TYPES)> piece_bitboards_ = { 0 };
            std::array<Bitboard, static_cast<int>(PieceColor::NUM_PIECE_COLORS)> color_bitboards_ = { 0 };
        public:
            const static std::size_t WIDTH = 8;
            const static std::size_t SIZE = WIDTH * WIDTH;

            class SquareGenerator;

            /**
             * Constructs a Board instance from a Square->Piece mapping.
             * I.e. for each pair (square, piece), piece is stored at square.
             *
             * TODO(theimer): expand.
             */
            Board(const std::unordered_map<Square, Piece>& piece_map);
            Board(const Board& copy_me);
            Board(Board&& move_me);
            ~Board();
            Board& operator=(const Board& copy_assign_me);
            Board& operator=(Board&& move_assign_me);

            SquareGenerator generateMatchingSquares(PieceType type, PieceColor color);

            /**
             * Returns true iff `square` is occupied on the Board.
             */
            bool squareIsOccupied(const Square& square) const;
            /**
             * Stores the piece described by `piece` at `square` on the Board.
             */
            void setPiece(const Piece& piece, const Square& square);

            friend void swapBoard(Board& board1, Board& board2);
    };

    class Board::SquareGenerator {
    	private:
    		Bitboard bitboard_;
    	public:
    		SquareGenerator(Bitboard bitboard);
    		bool hasNext();
    		Square next();
    };

    /**
     * "Swaps" the state of board1 with the state of board2.
     */
    void swapBoard(Board& board1, Board& board2);
}

// In the header file to support Board's map constructor.
namespace std {

	template <> struct hash<board::Square> {
		size_t operator()(const board::Square& x) const;
	};

	template <> struct hash<board::Piece> {
	    size_t operator()(const board::Piece& x) const;
	};
}

#endif  // BOARD_H_
