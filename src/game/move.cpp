// Copyright 2021 Alex Theimer

#include "game/move.h"

#include <map>
#include <vector>
#include <stdexcept>
#include <array>
#include <cmath>
#include <sstream>
#include <string>

#include "util/buffer.h"
#include "util/assert.h"

using board::Board;
using board::PieceColor;
using board::PieceType;
using board::Square;
using board::Piece;

using game::Move;

struct Diff {
    int row_diff;
    int col_diff;
};

/*
Given an initial Square and array of Diffs, fills a buffer with all *valid*
moves such that each destination Square is the sum of the initial Square
with the Diff.

A valid Move is any such that the destination:
    (1) is within the Board's bounds, and
    (2) does not contain a piece of the same color

@param color: the color of the piece at the Square
@param buffer: an Iterator at the first index of the buffer
@return: the number of Moves added to the buffer
*/
template <std::size_t SIZE>
std::size_t getMovesDiff(const Board& board, PieceColor color, Square square,
                         const std::array<Diff, SIZE>& diffs, Move* buffer) {
    std::size_t i = 0;
    for (Diff diff : diffs) {
        std::size_t row = square.row + diff.row_diff;
        std::size_t col = square.col + diff.col_diff;
        // add to the buffer if (1) in-bounds and (2) unoccupied by same color
        if (Square::isValidDims(row, col)) {
            Square new_square(row, col);
            if (!board.squareIsOccupiedColor(new_square, color)) {
                Move move = { square, new_square };
                buffer[i] = move;
                ++i;
            }
        }
    }
    return i;
}

/*
Given an initial Square and array of Diffs, fills a buffer with all *valid*
moves such that each destination Square is the sum of the initial Square with a
positive multiple of a Diff in the array.

For each Diff, Moves are generated with each coefficient
[1, 2, 3...] in increasing order until the first invalid Move is generated.

A valid Move is any such that the destination:
    (1) is within the Board's bounds, and
    (2) does not contain a piece of the same color

@param color: the color of the piece at the Square
@param buffer: an Iterator at the first index of the buffer
@return: the number of Moves added to the buffer
*/
template <std::size_t SIZE>
std::size_t getMovesVector(const Board& board, PieceColor color, Square square,
                           const std::array<Diff, SIZE>& vectors,
                           Move* buffer) {
    Move* move_ptr = buffer;
    for (Diff vec : vectors) {
        // TODO(theimer): worth changing the spec for this?
//        ASSERT((std::abs(vec.row_diff) <= 1) && (std::abs(vec.col_diff) <= 1),
//                "expected diffs <= 1; got: (" + std::to_string(vec.col_diff)
//                + ", " + std::to_string(vec.row_diff) +")");
        Square curr_sq = square;
        // keep adding multiples to each vec until an invalid square is reached.
        while (true) {
            std::size_t new_row = curr_sq.row + vec.row_diff;
            if (new_row >= Board::WIDTH) {
                break;
            }
            std::size_t new_col = curr_sq.col + vec.col_diff;
            if (new_col >= Board::WIDTH) {
                break;
            }
            curr_sq = Square(new_row, new_col);
            if (!board.squareIsOccupied(curr_sq)) {
                // square is empty!
                *move_ptr = (Move){ square, curr_sq };
                ++move_ptr;
            } else if (!board.squareIsOccupiedColor(curr_sq, color)) {
                // square occupied by enemy
                *move_ptr = (Move){ square, curr_sq };
                ++move_ptr;
                break;
            } else {
                // occupied by friendly!
                // jumps not allowed, so we're done with this Diff
                break;
            }
        }
    }
    return move_ptr - buffer;
}

/*
Fills a buffer with all valid moves by a king or pawn.
*/
std::size_t getMovesPawnKing(const Board& board, PieceColor color,
                             Square square, Move* buffer) {
    static const std::array<Diff, 8> diffs = {{
            {  1,  0 },
            {  0,  1 },
            {  1,  1 },
            { -1,  0 },
            {  0, -1 },
            { -1, -1 },
            {  1, -1 },
            { -1,  1 }
    }};

    return getMovesDiff(board, color, square, diffs, buffer);
}

/*
Fills a buffer with all valid moves by a knight.
*/
std::size_t getMovesKnight(const Board& board, PieceColor color,
                           Square square, Move* buffer) {
    static const std::array<Diff, 8> diffs = {{
            {  2,  1 },
            {  2, -1 },
            { -2,  1 },
            { -2, -1 },
            {  1,  2 },
            {  1, -2 },
            { -1,  2 },
            { -1, -2 }
    }};

    return getMovesDiff(board, color, square, diffs, buffer);
}

/*
Fills a buffer with all valid moves by a rook.
*/
std::size_t getMovesRook(const Board& board, PieceColor color,
                         Square square, Move* buffer) {
    static const std::array<Diff, 4> vectors = {{
            (Diff){  0,  1 },
            (Diff){  1,  0 },
            (Diff){ -1,  0 },
            (Diff){  0, -1 },
    }};

    return getMovesVector(board, color, square, vectors, buffer);
}

/*
Fills a buffer with all valid moves by a bishop.
*/
std::size_t getMovesBishop(const Board& board, PieceColor color,
                           Square square, Move* buffer) {
    static const std::array<Diff, 4> vectors = {{
            (Diff){  1,  1 },
            (Diff){  1, -1 },
            (Diff){ -1,  1 },
            (Diff){ -1, -1 },
    }};

    return getMovesVector(board, color, square, vectors, buffer);
}

/*
Fills a buffer with all valid moves by a queen.
*/
std::size_t getMovesQueen(const Board& board, PieceColor color,
                          Square square, Move* buffer) {
    static const std::array<Diff, 8> vectors = {{
            (Diff){  1,  1 },
            (Diff){  1, -1 },
            (Diff){ -1,  1 },
            (Diff){ -1, -1 },
            (Diff){  0,  1 },
            (Diff){  1,  0 },
            (Diff){ -1,  0 },
            (Diff){  0, -1 },
    }};

    return getMovesVector(board, color, square, vectors, buffer);
}

std::string std::to_string(game::Move move) {
    std::stringstream sstr;
    sstr << "Move(from: " << std::to_string(move.from)
         << ", to: " << std::to_string(move.to) << ")";
    return sstr.str();
}

bool game::operator==(Move lhs, Move rhs) {
    return (lhs.from == rhs.from) && (lhs.to == rhs.to);
}

std::ostream& game::operator<<(std::ostream& out, Move move) {
    out << std::to_string(move);
    return out;
}

std::size_t game::getPieceMoves(const Board& board, PieceColor color,
                                Square square, Move* buffer) {
    // TODO(theimer): better to just map function pointers?
    PieceType type = board.getPieceType(square);
    switch (type) {
    case PieceType::PAWN:
    case PieceType::KING:
        return getMovesPawnKing(board, color, square, buffer);
    case PieceType::BISHOP:
        return getMovesBishop(board, color, square, buffer);
    case PieceType::KNIGHT:
        return getMovesKnight(board, color, square, buffer);
    case PieceType::QUEEN:
        return getMovesQueen(board, color, square, buffer);
    case PieceType::ROOK:
        return getMovesRook(board, color, square, buffer);
    default:
        throw std::invalid_argument(
                "unhandled PieceType: " + std::to_string(type));
    }
}

std::size_t game::getAllMoves(const Board& board, PieceColor color,
                              Move* buffer) {
    // get all occupied squares, then get the valid moves from those squares
    util::Buffer<Square, Board::SIZE> occupied_buffer;
    std::size_t num_occupied =
            board.getOccupiedSquares(color, occupied_buffer.start());
    Move* next_move_slot = buffer;
    for (std::size_t i = 0; i < num_occupied; ++i) {
        next_move_slot += getPieceMoves(
                board, color, occupied_buffer.get(i), next_move_slot);
    }
    return next_move_slot - buffer;
}

// TODO(theimer): board->index recomputation below!
// TODO(theimer): these are super inefficient in-general

std::optional<Piece> game::makeMove(Board* board, Move move) {
    ASSERT(board->squareIsOccupied(move.from),
            "unoccupied square: " + std::to_string(move.from));
    // make sure move.to is unoccupied / occupied by a different color.
    ASSERT(!board->squareIsOccupiedColor(
                move.to, board->getPieceColor(move.from)),
           "to square occupied by same color: " + std::to_string(move));
    // I really don't want an "empty" PieceType and PieceColor, but
    //     those might be more efficient than optioanls...
    std::optional<Piece> removed = board->squareIsOccupied(move.to)
                                 ? std::make_optional(board->getPiece(move.to))
                                 : std::optional<Piece>();
    board->movePieceOverwrite(move.from, move.to);
    return removed;
}

void game::unmakeMove(Board* board, Move move,
                      std::optional<Piece> replacement) {
    ASSERT(board->squareIsOccupied(move.to),
            "unoccupied square: " + std::to_string(move.to));
    ASSERT(!board->squareIsOccupied(move.from),
            "occupied square: " + std::to_string(move.from));

    // make sure no residual "if" left over in non-debug compilation
    #ifdef DEBUG
    if (replacement.has_value()) {
        ASSERT(board->getPieceColor(move.to) != replacement->color,
                "piece has same color as replacement: "
                + std::to_string(move.to));
    }
    # endif

    // note: not "overwrite" because unmaking a move should move
    //       a piece into an empty square
    board->movePiece(move.to, move.from);
    if (replacement.has_value()) {
        board->setPiece(*replacement, move.to);
    }
}

bool game::isValidMove(const Board& board, PieceColor color, Move move) {
    util::Buffer<Move, MAX_NUM_MOVES_PLY> move_buffer;
    std::size_t num_moves = getAllMoves(board, color, move_buffer.start());
    for (std::size_t i = 0; i < num_moves; ++i) {
        if (move == move_buffer.get(i)) {
            return true;
        }
    }
    return false;
}
