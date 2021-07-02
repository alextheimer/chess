// Copyright 2021 Alex Theimer

#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "board/piece.h"

using board::Piece;
using board::PieceColor;
using board::PieceType;

/*
~~~ Test Partitions ~~~
compressPiece/decompressPiece
    type: { all piece types }
    color:{ all piece colors }
*/

/*
Compresses/decompresses Pieces of every possible combination of
PieceType/PieceColor, then compares the decompression result against
the expected result.

Covers:
    compressPiece/decompressPiece
        type: { all piece types }
        color:{ all piece colors }
*/
TEST(PieceTest, CompressDecompressTest) {
    for (std::size_t icolor = 0;
             icolor < static_cast<std::size_t>(PieceColor::NUM_PIECE_COLORS);
             ++icolor) {
        for (std::size_t itype = 0;
                 itype < static_cast<std::size_t>(PieceType::NUM_PIECE_TYPES);
                 ++itype) {
            Piece piece = { static_cast<PieceType>(itype),
                            static_cast<PieceColor>(icolor) };
            Piece decoded = board::decompressPiece(board::compressPiece(piece));
            ASSERT_EQ(piece, decoded);
        }
    }
}
