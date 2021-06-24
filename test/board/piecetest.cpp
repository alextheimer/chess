#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "board/piece.h"

using namespace board;

/*
~~~ Test Partitions ~~~
compressPiece/decompressPiece
    type: { all piece types }
    color:{ all piece colors }
*/

/*
Covers:
    compressPiece/decompressPiece
        type: { all piece types }
        color:{ all piece colors }
*/
TEST(PieceTest, CompressDecompressTest) {
    for (int icolor = 0; icolor < static_cast<int>(PieceColor::NUM_PIECE_COLORS); ++icolor) {
        for (int itype = 0; itype < static_cast<int>(PieceType::NUM_PIECE_TYPES); ++itype) {
            Piece piece = { static_cast<PieceType>(itype), static_cast<PieceColor>(icolor) };
            Piece decoded = decompressPiece(compressPiece(piece));
            ASSERT_EQ(piece, decoded);
        }
    }
}
