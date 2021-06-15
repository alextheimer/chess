#include <vector>
#include <unordered_map>

#include "gtest/gtest.h"
#include "board/board.h"

using namespace board;

TEST(BoardTest, BasicOccupancyTest) {
    std::unordered_map<Square, Piece> piece_map {
        {Square(0, 0), (Piece){PieceType::KING, PieceColor::BLACK}}
    };
    Board board(piece_map);
    ASSERT_TRUE(board.squareIsOccupied(Square(0,0)));
    ASSERT_FALSE(board.squareIsOccupied(Square(0, 1)));
}
