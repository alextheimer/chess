// Copyright 2021 Alex Theimer

// Implementations of Board-related templates.

#include "util/bitops.h"

/*
Fills a buffer with all Squares such that each contains a 1 on the Bitboard.
@param buffer: a random-access iterator at the first index of the buffer.
@return: the number of Squares added to the buffer.
*/
template<typename RandomAccessIter>
std::size_t bitboardToSquares(board::Bitboard board, RandomAccessIter buffer) {
     RandomAccessIter begin = buffer;
    // i.e. "while 1's exist on the board"
    // TODO(theimer): might vectorize with popcount-based for loop
    while (board > 0) {
        std::size_t index = util::popLowestBit(&board);
        *buffer = board::Square::indexToSquare(index);
        ++buffer;
    }   
    return buffer - begin;
}

template<typename RandomAccessIter>
std::size_t board::Board::getOccupiedSquares(board::PieceColor color,
                                             RandomAccessIter buffer) const {
    board::Bitboard board = color_bitboards_[static_cast<size_t>(color)];
    return bitboardToSquares(board, buffer);
}

template<typename RandomAccessIter>
std::size_t board::Board::getOccupiedSquares(board::PieceType type,
                                             RandomAccessIter buffer) const {
    board::Bitboard board = piece_bitboards_[static_cast<size_t>(type)];
    return bitboardToSquares(board, buffer);
}

template<typename RandomAccessIter>
std::size_t board::Board::getOccupiedSquares(RandomAccessIter buffer) const {
    // get the union of all color bitboards, then convert to squares.
    board::Bitboard board = 0;
    for (std::size_t i = 0;
            i < static_cast<std::size_t>(board::PieceColor::NUM_PIECE_COLORS);
            ++i) {
        board |= color_bitboards_[i];
    }   
    return bitboardToSquares(board, buffer);
}

