#include <vector>

#include "gtest/gtest.h"
#include "board/piece.h"

using namespace board;

TEST(PieceTest, CompressionTest) {
    std::vector<Piece> pieces {
        (Piece){PieceType::KING, PieceColor::BLACK},
        (Piece){PieceType::PAWN, PieceColor::WHITE},
        (Piece){PieceType::BISHOP, PieceColor::BLACK}
    };

    for (Piece piece : pieces) {
        Piece decompressed = decompressPiece(compressPiece(piece));
        ASSERT_EQ(piece, decompressed);
    }
}
