#include <vector>
#include <unordered_map>

#include "gtest/gtest.h"
#include "board/board.h"

using namespace board;

TEST(BoardTest, BasicOccupancyTest) {
    std::unordered_map<Square, Piece> piece_map {
        {Square(0, 0), Piece(PieceType::KING, PieceColor::BLACK)}
    };
    Board board(piece_map);
    ASSERT_TRUE(board.squareIsOccupied(Square(0,0)));
    ASSERT_FALSE(board.squareIsOccupied(Square(0, 1)));
}

TEST(BoardTest, SquareGeneratorTest) {
    auto exhaust_gen_func = [](Board::SquareGenerator& gen, std::vector<Square>& vec) {
        while (gen.hasNext()) {
            vec.push_back(gen.next());
        }
    };

    std::unordered_map<Square, Piece> piece_map {
        {Square(0, 0), Piece(PieceType::KING, PieceColor::BLACK)},
        {Square(3, 4), Piece(PieceType::KING, PieceColor::BLACK)},
        {Square(0, 1), Piece(PieceType::PAWN, PieceColor::WHITE)},
        {Square(3, 3), Piece(PieceType::PAWN, PieceColor::BLACK)}
    };

    Board board(piece_map);

    Board::SquareGenerator white_pawn_gen = board.generateMatchingSquares(PieceType::PAWN, PieceColor::WHITE);
    Board::SquareGenerator black_pawn_gen = board.generateMatchingSquares(PieceType::PAWN, PieceColor::BLACK);
    Board::SquareGenerator black_king_gen = board.generateMatchingSquares(PieceType::KING, PieceColor::BLACK);

    std::vector<Square> vec;
    exhaust_gen_func(black_king_gen, vec);
    ASSERT_EQ(2, vec.size());
}
