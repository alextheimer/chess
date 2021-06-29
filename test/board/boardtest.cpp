#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "gtest/gtest.h"
#include "board/board.h"
#include "util/buffer.h"
#include "util/assert.h"

using board::Square;
using board::Piece;
using board::Board;
using board::PieceType;
using board::PieceColor;

/*
~~~ Test Partitions ~~~
squareIsOccupied
    square: (0,0), (0,7) (7,0), (7,7), other
    square: empty, nonempty
    board: single piece, no pieces, multiple pieces
    square: {occupied by all piece types/colors}
setPiece
    square: (0,0), (0,7) (7,0), (7,7), other
    piece: {all piece types/colors}
    board: single piece, no pieces, multiple pieces
movePiece
    from: (0,0), (0,7) (7,0), (7,7), other
    to: (0,0), (0,7) (7,0), (7,7), other
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
TODO(theimer): test overwrite variants of move/setPiece
*/

/*
Returns a (heap-allocated) vector of all valid, unique Board Squares.
(Used to instantiate ALL_SQUARES below.)
*/
std::unique_ptr<std::vector<Square>> getAllSquares() {
    auto squares = std::unique_ptr<std::vector<Square>>(new std::vector<Square>);
    for (int irow = 0; irow < static_cast<int>(Board::WIDTH); ++irow) {
        for (int icol = 0; icol < static_cast<int>(Board::WIDTH); ++icol) {
            squares->push_back(Square(irow, icol));
        }
    }
    return squares;
}

// TODO(theimer): move this to Board?
// vector of all valid, unique Board Squares.
static const std::unique_ptr<std::vector<Square>> ALL_SQUARES = getAllSquares();

/*
Instantiates Boards each with a set of Pieces, then confirms that
Board::squareIsOccupied returns the expected result for every square.

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

    // for each piece_map, make sure squareIsOccuped returns
    //     the expected result for every square
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
Instantiates empty Boards, then sets Pieces (via Board::setPiece) onto each.
Then, for each...
    empty square: confirm the square is empty (via Board::squareIsOccupied)
    nonempty square: confirm Board::getPiece returns the expected piece.

Covers:
    getPiece
        square: (0,0), (0,7) (7,0), (7,7), other
        piece: {all piece types/colors}
        board: single piece, multiple pieces
    setPiece
        square: (0,0), (0,7) (7,0), (7,7), other
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
        // set all the pieces from the map
        for (auto pair : piece_map) {
            board.setPiece(pair.second, pair.first);
        }
        // step through all squares and confirm their state matches what we expect
        for (Square& square : *ALL_SQUARES) {
            auto piece_iter = piece_map.find(square);
            if (piece_iter != piece_map.end()) {
                // square is occupied!
                ASSERT_EQ(piece_iter->second, board.getPiece(square));
            } else {
                // unoccupied
                ASSERT_FALSE(board.squareIsOccupied(square));
            }
        }
    }
}

/*
Instantiates Boards each with a set of Pieces, then repositions/overwrites
some via Board::movePiece. For every Square, confirms Board::getPiece
and Board::squareIsOccupied return the expected results.

Covers:
    movePiece
        from: (0,0), (0,7) (7,0), (7,7), other
        to: (0,0), (0,7) (7,0), (7,7), other
        board: single piece, multiple pieces
*/
TEST(BoardTest, MoveTest) {

    struct Move {
        Square from;
        Square to;
    };

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
                            (Move){Square(0, 7), Square(3, 2)},
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
        // copy of piece_map used to find the expected final Board setup
        std::unordered_map<Square, Piece> piece_tracker = spec.piece_map;
        for (Move& move : spec.moves) {
            // update the tracker
            Piece moved = piece_tracker[move.from];
            piece_tracker.erase(move.from);
            piece_tracker[move.to] = moved;

            // actually move Piece on the board
            board.movePiece(move.from, move.to);

            // make sure tracker and board agree
            for (Square& square : *ALL_SQUARES) {
                auto piece_iter = piece_tracker.find(square);
                if (piece_iter != piece_tracker.end()) {
                    // square is occupied!
                    ASSERT_EQ(piece_iter->second, board.getPiece(square));
                } else {
                    // unoccupied
                    ASSERT_FALSE(board.squareIsOccupied(square));
                }
            }
        }
    }
}

/*
Instantiates Boards each with their own set of Pieces, then calls Board::getOccupiedSquares.
Confirms result is expected.

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
            // Store all pieces of the same color.
            //     These are removed as we encounter them.
            std::unordered_set<Square> square_set;
            for (auto pair : piece_map) {
                if (pair.second.color == static_cast<PieceColor>(icolor)) {
                    square_set.insert(pair.first);
                }
            }
            PieceColor color = static_cast<PieceColor>(icolor);
            size_t num_squares = board.getOccupiedSquares(color, occupied_buffer.start());
            for (int isquare = 0; isquare < static_cast<int>(num_squares); ++isquare) {
                Square square = occupied_buffer.get(isquare);
                // make sure this square still exists in the set
                ASSERT_NE(square_set.end(), square_set.find(square));
                // make sure it's occupied by the expected piece
                ASSERT_EQ(piece_map.at(square), board.getPiece(square));
                // essentially: mark this square as "visited"
                square_set.erase(square);
            }
            // make sure all squares were accounted for
            ASSERT_EQ(static_cast<std::size_t>(0), square_set.size());
        }
    }
}
