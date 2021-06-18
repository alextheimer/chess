#include "game/move.h"

#include <map>
#include <vector>
#include <stdexcept>

using namespace board;

struct Diff {
    int row_diff;
    int col_diff;
};

bool isInBounds(std::size_t row, std::size_t col) {
    return (row < Board::WIDTH) && (col < Board::WIDTH);
}

std::size_t getMovesPawn(Board& board, PieceColor color, Square square, Move* buffer) {
    static const std::vector<Diff> diffs = {
            {  1, 0 },
            {  0, 1 },
            {  1, 1 },
            { -1, 0 },
            {  0, -1 },
            { -1, -1 },
            {  1, -1 },
            { -1,  1 }
    };

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

std::size_t game::getPieceMoves(Board& board, PieceColor color, Square square, Move * buffer) {
    // TODO(theimer): better to just map function pointers?
    switch (board.getPieceType(square)) {
    case PieceType::PAWN:
        return getMovesPawn(board, color, square, buffer);
    default:
        // TODO(theimer): add the others!
        // TODO(theimer): add piece type string
        throw std::invalid_argument("unhandled PieceType: TODO");
    }
}

std::size_t game::getAllMoves(Board& board, PieceColor color, Move * buffer) {
    // instantiate as a byte array so nothing is automatically constructed
    uint8_t raw_buffer[Board::SIZE * sizeof(Square)];
    Square* occupied_buffer = reinterpret_cast<Square*>(raw_buffer);

    std::size_t num_occupied = board.getOccupiedSquares(color, occupied_buffer);
    Move * nextMoveSlot = buffer;
    std::size_t num_moves = 0;
    for (int i = 0; i < num_occupied; ++i) {
        nextMoveSlot += getPieceMoves(board, color, occupied_buffer[i], nextMoveSlot);
    }
    return nextMoveSlot - buffer;
}

void game::makeMove(Board& board, Move& move) {
    // TODO(theimer): assertions / rule stuff
    board.movePiece(move);
}

void game::unmakeMove(Board& board, Move& move, Piece replacement) {
    // TODO(theimer): assertions
    Move backwards = { move.to, move.from };
    board.movePiece(backwards);
    board.setPiece(replacement, move.to);
}
