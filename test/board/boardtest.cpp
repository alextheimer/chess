#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

#include "gtest/gtest.h"
#include "board/board.h"

using namespace board;

TEST(BoardTest, OccupancyTest) {
    std::unordered_map<Square, Piece> piece_map {
        {Square(0, 0), (Piece){PieceType::KING, PieceColor::BLACK}}
    };
    Board board(piece_map);
    ASSERT_TRUE(board.squareIsOccupied(Square(0,0)));
    ASSERT_FALSE(board.squareIsOccupied(Square(0, 1)));
}

// get occupied squares
// square is occupied color

TEST(BoardTest, SetGetTest) {

    std::unordered_map<Square, Piece> piece_map {
        { Square(0, 0), (Piece){ PieceType::KING, PieceColor::BLACK } }
    };

    std::vector<Square> empty_list = {
            Square(0, 1),
            Square(1, 0),
            Square(1, 1)
    };

    Board board;

    for (auto pair : piece_map) {
        board.setPiece(pair.second, pair.first);
        ASSERT_TRUE(board.squareIsOccupied(pair.first));
    }

    for (Square& sq : empty_list) {
        ASSERT_FALSE(board.squareIsOccupied(sq));
    }

    for (auto pair : piece_map) {
        ASSERT_TRUE(board.squareIsOccupied(pair.first));
    }

}

TEST(BoardTest, MoveTest) {

    std::unordered_map<Square, Piece> piece_map = {
            { Square(0, 0), (Piece){ PieceType::KING, PieceColor::BLACK } }
    };

    std::vector<Move> moves = {
            { Square(0, 0), Square(7, 7) }
    };

    Board board(piece_map);
    for (Move& move : moves) {
        Piece moved = board.getPiece(move.from);
        board.movePiece(move);
        ASSERT_TRUE(board.squareIsOccupied(move.to));
        ASSERT_FALSE(board.squareIsOccupied(move.from));
        ASSERT_EQ(moved, board.getPiece(move.to));
    }
}

TEST(BoardTest, GetOccupiedSquaresTest) {
    static const size_t SIZE = 5;
    std::unordered_map<Square, Piece> piece_map = {
            { Square(0, 0), (Piece){ PieceType::KING, PieceColor::BLACK } },
            { Square(4, 5), (Piece){ PieceType::QUEEN, PieceColor::BLACK } },
            { Square(7, 7), (Piece){ PieceType::PAWN, PieceColor::WHITE } },
            { Square(0, 1), (Piece){ PieceType::ROOK, PieceColor::WHITE } },
            { Square(1, 1), (Piece){ PieceType::KNIGHT, PieceColor::WHITE } }
    };
    assert(SIZE == piece_map.size());

    std::unordered_set<Square> square_set;
    for (auto pair : piece_map) {
        square_set.insert(pair.first);
    }

    Board board(piece_map);
    uint8_t buffer[SIZE * sizeof(Square)];
    Square* occupied_buffer = reinterpret_cast<Square*>(buffer);

    for (int icolor = 0; icolor < static_cast<int>(PieceColor::NUM_PIECE_COLORS); ++icolor) {
        PieceColor color = static_cast<PieceColor>(icolor);
        size_t num_squares = board.getOccupiedSquares(color, occupied_buffer);
        for (int isquare = 0; isquare < num_squares; ++isquare) {
            Square square = occupied_buffer[isquare];
            ASSERT_NE(square_set.end(), square_set.find(square));
            square_set.erase(square);
        }
    }

    ASSERT_EQ(0, square_set.size());
}
