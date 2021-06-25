#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "gtest/gtest.h"
#include "board/board.h"
#include "util/buffer.h"
#include "util/assert.h"

using namespace board;

/*
~~~ Test Partitions ~~~
squareIsOccupied
    square: (0,0), (0,7) (7,0), (7,7), other
    square: empty, nonempty
    board: single piece, no pieces, multiple pieces
    square: {occupied by all piece types/colors}
setPiece
    square: (0,0), (0,7) (7,0), (7,7), other
    square: empty, nonempty
    piece: {all piece types/colors}
    board: single piece, no pieces, multiple pieces
movePiece
    from: (0,0), (0,7) (7,0), (7,7), other
    to: (0,0), (0,7) (7,0), (7,7), other
    to: empty, nonempty
    board: single piece, multiple pieces
getPiece
    square: (0,0), (0,7) (7,0), (7,7), other
    piece: {all piece types/colors}
    board: single piece, multiple pieces
removePiece  // TODO(theimer): add these tests!
    square: (0,0), (0,7) (7,0), (7,7), other
    piece: {all piece types/colors}
    board: single piece, multiple pieces
getOccupiedSquares
    board: single piece, no pieces, multiple pieces
    board: occupied squares at (0,0), (0,7) (7,0), (7,7), other
    board: { contains various piecetypes/colors }
*/

std::unique_ptr<std::vector<Square>> getAllSquares() {
    auto squares = std::unique_ptr<std::vector<Square>>(new std::vector<Square>);
    for (int irow = 0; irow < Board::WIDTH; ++irow) {
        for (int icol = 0; icol < Board::WIDTH; ++icol) {
            squares->push_back(Square(irow, icol));
        }
    }
    return squares;
}

static const std::unique_ptr<std::vector<Square>> ALL_SQUARES = getAllSquares();

/*
Covers:
    squareIsOccupied
        square: (0,0), (0,7) (7,0), (7,7), other
        square: empty, nonempty
        board: single piece, no pieces, multiple pieces
*/
TEST(BoardTest, SquareIsOccupiedTest) {
    std::vector<std::unordered_map<Square, Piece>> piece_maps = {
            {},
            {{Square(0, 0), (Piece){PieceType::KING, PieceColor::BLACK}}},
            {{Square(7, 7), (Piece){PieceType::PAWN, PieceColor::WHITE}}},
            {
                    {Square(0, 0), (Piece){PieceType::QUEEN, PieceColor::WHITE}},
                    {Square(0, 7), (Piece){PieceType::BISHOP, PieceColor::WHITE}},
                    {Square(7, 0), (Piece){PieceType::ROOK, PieceColor::BLACK}},
                    {Square(7, 7), (Piece){PieceType::KNIGHT, PieceColor::WHITE}},
            },
            {
                    {Square(4, 5), (Piece){PieceType::QUEEN, PieceColor::BLACK}},
                    {Square(0, 7), (Piece){PieceType::KING, PieceColor::WHITE}},
                    {Square(6, 1), (Piece){PieceType::PAWN, PieceColor::BLACK}},
                    {Square(3, 2), (Piece){PieceType::PAWN, PieceColor::BLACK}},
            },
    };

    for (auto& piece_map : piece_maps) {
        Board board(piece_map);
        for (Square& square : *ALL_SQUARES) {
            ASSERT_EQ(
                (piece_map.find(square) != piece_map.end()),
                board.squareIsOccupied(square)
            );
        }
    }
}

/*
Covers:
    getPiece
        square: (0,0), (0,7) (7,0), (7,7), other
        piece: {all piece types/colors}
        board: single piece, multiple pieces
    setPiece
        square: (0,0), (0,7) (7,0), (7,7), other
        square: empty  // TODO(theimer): need nonempty tests!,
        piece: {all piece types/colors}
        board: single piece, no pieces, multiple pieces
*/
TEST(BoardTest, GetSetTest) {

    std::vector<std::unordered_map<Square, Piece>> piece_maps = {
            {},
            {{Square(0, 0), (Piece){PieceType::KING, PieceColor::BLACK}}},
            {{Square(7, 7), (Piece){PieceType::PAWN, PieceColor::WHITE}}},
            {
                    {Square(0, 0), (Piece){PieceType::QUEEN, PieceColor::WHITE}},
                    {Square(0, 7), (Piece){PieceType::BISHOP, PieceColor::WHITE}},
                    {Square(7, 0), (Piece){PieceType::ROOK, PieceColor::BLACK}},
                    {Square(7, 7), (Piece){PieceType::KNIGHT, PieceColor::WHITE}},
            },
            {
                    {Square(4, 5), (Piece){PieceType::QUEEN, PieceColor::BLACK}},
                    {Square(0, 7), (Piece){PieceType::KING, PieceColor::WHITE}},
                    {Square(6, 1), (Piece){PieceType::PAWN, PieceColor::BLACK}},
                    {Square(3, 2), (Piece){PieceType::PAWN, PieceColor::BLACK}},
            },
    };

    for (auto& piece_map : piece_maps) {
        Board board;
        for (auto pair : piece_map) {
            board.setPiece(pair.second, pair.first);
        }
        for (Square& square : *ALL_SQUARES) {
            auto piece_iter = piece_map.find(square);
            if (piece_iter != piece_map.end()) {
                // square is occupied!
                ASSERT_EQ(piece_iter->second, board.getPiece(square));
            } else {
                ASSERT_FALSE(board.squareIsOccupied(square));
            }
        }
    }

}

/*
Covers:
    movePiece
        from: (0,0), (0,7) (7,0), (7,7), other
        to: (0,0), (0,7) (7,0), (7,7), other
        to: empty, nonempty
        board: single piece, multiple pieces
*/
TEST(BoardTest, MoveTest) {

    struct TestSpec {
        std::unordered_map<Square, Piece> piece_map;
        std::vector<Move> moves;
    };

    std::vector<TestSpec> specs = {
            {
                    {{Square(0, 0), (Piece){PieceType::KING, PieceColor::BLACK}}},
                    {(Move){Square(0, 0), Square(7, 7)}}
            },
            {
                    {{Square(7, 7), (Piece){PieceType::PAWN, PieceColor::WHITE}}},
                    {(Move){Square(7, 7), Square(0, 0)}}
            },
            {
                    {
                            {Square(0, 0), (Piece){PieceType::QUEEN, PieceColor::WHITE}},
                            {Square(0, 7), (Piece){PieceType::BISHOP, PieceColor::WHITE}},
                            {Square(7, 0), (Piece){PieceType::ROOK, PieceColor::BLACK}},
                            {Square(7, 7), (Piece){PieceType::KNIGHT, PieceColor::WHITE}},
                    },
                    {
                            (Move){Square(0, 0), Square(0, 7)},  // overwrite!
                            (Move){Square(7, 7), Square(3, 2)},
                            (Move){Square(7, 0), Square(2, 3)},
                    }
            },
            {
                    {
                            {Square(4, 5), (Piece){PieceType::QUEEN, PieceColor::BLACK}},
                            {Square(1, 6), (Piece){PieceType::KING, PieceColor::WHITE}},
                            {Square(6, 1), (Piece){PieceType::PAWN, PieceColor::BLACK}},
                            {Square(3, 2), (Piece){PieceType::PAWN, PieceColor::BLACK}},
                    },
                    {
                            (Move){Square(4, 5), Square(2, 3)},
                            (Move){Square(1, 6), Square(7, 0)},
                            (Move){Square(6, 1), Square(0, 0)},
                            (Move){Square(3, 2), Square(5, 5)},
                    }
            },
    };

    for (TestSpec spec : specs) {
        Board board(spec.piece_map);
        std::unordered_map<Square, Piece> piece_tracker = spec.piece_map;
        for (Move& move : spec.moves) {
            // update the tracker
            Piece moved = piece_tracker[move.from];
            piece_tracker.erase(move.from);
            piece_tracker[move.to] = moved;

            // actually move on the board
            board.movePiece(move);

            // make sure tracker and board agree
            for (Square& square : *ALL_SQUARES) {
                auto piece_iter = piece_tracker.find(square);
                if (piece_iter != piece_tracker.end()) {
                    // square is occupied!
                    ASSERT_EQ(piece_iter->second, board.getPiece(square));
                } else {
                    ASSERT_FALSE(board.squareIsOccupied(square));
                }
            }
        }
    }
}
/*
Covers:
    getOccupiedSquares
        board: single piece, no pieces, multiple pieces
        board: occupied squares at (0,0), (0,7) (7,0), (7,7), other
        board: { contains various piecetypes/colors }
*/
TEST(BoardTest, GetOccupiedSquaresTest) {
    std::vector<std::unordered_map<Square, Piece>> piece_maps = {
            {/* intentionally empty */},
            {
                { Square(0, 0), (Piece){ PieceType::KING, PieceColor::BLACK } }
            },
            {
                { Square(0, 0), (Piece){ PieceType::BISHOP, PieceColor::BLACK } },
                { Square(0, 7), (Piece){ PieceType::QUEEN, PieceColor::WHITE } },
                { Square(7, 0), (Piece){ PieceType::ROOK, PieceColor::BLACK } },
                { Square(7, 7), (Piece){ PieceType::KNIGHT, PieceColor::WHITE } },
            },
            {
                { Square(0, 4), (Piece){ PieceType::PAWN, PieceColor::BLACK } },
                { Square(2, 3), (Piece){ PieceType::QUEEN, PieceColor::WHITE } },
                { Square(7, 1), (Piece){ PieceType::ROOK, PieceColor::BLACK } },
                { Square(1, 6), (Piece){ PieceType::PAWN, PieceColor::BLACK } },
            }
    };


    for (std::unordered_map<Square, Piece> piece_map : piece_maps) {
        Board board(piece_map);
        util::Buffer<Square, Board::SIZE> occupied_buffer;
        for (int icolor = 0; icolor < static_cast<int>(PieceColor::NUM_PIECE_COLORS); ++icolor) {
            // store all pieces of the same color
            std::unordered_set<Square> square_set;
            for (auto pair : piece_map) {
                if (pair.second.color == static_cast<PieceColor>(icolor)) {
                    square_set.insert(pair.first);
                }
            }
            PieceColor color = static_cast<PieceColor>(icolor);
            size_t num_squares = board.getOccupiedSquares(color, occupied_buffer.start());
            for (int isquare = 0; isquare < num_squares; ++isquare) {
                Square square = occupied_buffer.get(isquare);
                // make sure this square still exists in the set
                ASSERT_NE(square_set.end(), square_set.find(square));
                // make sure it's occupied by the expected piece
                ASSERT_EQ(piece_map.at(square), board.getPiece(square));
                square_set.erase(square);
            }
            // make sure all squares were accounted for
            ASSERT_EQ(0, square_set.size());
        }
    }
}
