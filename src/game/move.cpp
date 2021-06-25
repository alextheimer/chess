#include "game/move.h"
#include "util/buffer.h"
#include "util/assert.h"

#include <map>
#include <vector>
#include <stdexcept>
#include <array>
#include <cmath>
#include <sstream>

using namespace board;
using namespace game;

struct Diff {
    int row_diff;
    int col_diff;
};

bool isInBounds(std::size_t row, std::size_t col) {
    return (row < Board::WIDTH) && (col < Board::WIDTH);
}

template <std::size_t SIZE>
std::size_t getMovesDiff(Board& board, PieceColor color, Square square, const std::array<Diff, SIZE>& diffs, Move* buffer) {
    std::size_t i = 0;
    for (Diff diff : diffs) {
        std::size_t row = square.row + diff.row_diff;
        std::size_t col = square.col + diff.col_diff;
        if (isInBounds(row, col)) {
            Square new_square(row, col);
            if (!board.squareIsOccupiedColor(new_square, color)) {
                // TODO(theimer): should be an rvalue!!
                Move move = { square, new_square };
                buffer[i] = move;
                ++i;
            }
        }
    }
    return i;
}

std::size_t getMovesPawnKing(Board& board, PieceColor color, Square square, Move* buffer) {
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

std::size_t getMovesKnight(Board& board, PieceColor color, Square square, Move* buffer) {
    static const std::array<Diff, 8> diffs = {{
            {  3,  1 },
            {  3, -1 },
            { -3,  1 },
            { -3, -1 },
            {  1,  3 },
            {  1, -3 },
            { -1,  3 },
            { -1, -3 }
    }};

    return getMovesDiff(board, color, square, diffs, buffer);
}


template <std::size_t SIZE>
std::size_t getMovesVector(Board& board, PieceColor color, Square square, const std::array<Diff, SIZE>& vectors, Move* buffer) {
    Move* move_ptr = buffer;
    for (Diff vec : vectors) {
        ASSERT((std::abs(vec.row_diff) <= 1) && (std::abs(vec.col_diff) <= 1),
                "expected diffs <= 1; got: (" + std::to_string(vec.col_diff) + ", "
                + std::to_string(vec.row_diff) +")");
        Square curr_sq = square;
        while (true) {
            std::size_t new_row = curr_sq.row + vec.row_diff;
            if (new_row >= Board::WIDTH) {
                break;
            }
            std::size_t new_col = curr_sq.col + vec.col_diff;
            if (new_col >= Board::WIDTH) {
                break;
            }
            Square pend_sq(new_row, new_col);
            // TODO(theimer): need a "getOppositeColor" to make this more clear
            if (!board.squareIsOccupied(pend_sq)) {
                // square is empty!
                *move_ptr = (Move){ square, pend_sq };
                ++move_ptr;
            } else if (!board.squareIsOccupiedColor(pend_sq, color)) {
                // square occupied by enemy
                *move_ptr = (Move){ square, pend_sq };
                ++move_ptr;
                break;
            } else {
                // occupied by friendly; jumps not allowed, so we're done
                break;
            }
        }
    }

    return move_ptr - buffer;
}

std::size_t getMovesRook(Board& board, PieceColor color, Square square, Move* buffer) {
    static const std::array<Diff, 4> vectors = {{
            (Diff){  0,  1 },
            (Diff){  1,  0 },
            (Diff){ -1,  0 },
            (Diff){  0, -1 },
    }};

    return getMovesVector(board, color, square, vectors, buffer);
}

std::size_t getMovesBishop(Board& board, PieceColor color, Square square, Move* buffer) {
    static const std::array<Diff, 4> vectors = {{
            (Diff){  1,  1 },
            (Diff){  1, -1 },
            (Diff){ -1,  1 },
            (Diff){ -1, -1 },
    }};

    return getMovesVector(board, color, square, vectors, buffer);
}

std::size_t getMovesQueen(Board& board, PieceColor color, Square square, Move* buffer) {
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

bool game::operator==(const Move& lhs, const Move& rhs) {
    return (lhs.from == rhs.from) && (lhs.to == rhs.to);
}

std::string Move::toString() const {
    std::stringstream sstr;
    sstr << "Move(from: " << from << ", to: " << to << ")";
    return sstr.str();
}

std::ostream& game::operator<<(std::ostream& out, const Move& move) {
    out << move.toString();
    return out;
}

std::size_t game::getPieceMoves(Board& board, PieceColor color, Square square, Move * buffer) {
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
        throw std::invalid_argument("unhandled PieceType: " + toString(type));
    }
}

std::size_t game::getAllMoves(Board& board, PieceColor color, Move * buffer) {
    // instantiate as a byte array so nothing is automatically constructed
    util::Buffer<Square, Board::SIZE> occupied_buffer;

    std::size_t num_occupied = board.getOccupiedSquares(color, occupied_buffer.start());
    Move * nextMoveSlot = buffer;
    std::size_t num_moves = 0;
    for (int i = 0; i < num_occupied; ++i) {
        nextMoveSlot += getPieceMoves(board, color, occupied_buffer.get(i), nextMoveSlot);
    }
    return nextMoveSlot - buffer;
}

void game::makeMove(Board& board, Move& move) {
    // TODO(theimer): assertions / rule stuff
    board.movePiece(move.from, move.to);
}

void game::unmakeMove(Board& board, Move& move, Piece replacement) {
    // TODO(theimer): assertions
    board.movePiece(move.to, move.from);
    board.setPiece(replacement, move.to);
}
