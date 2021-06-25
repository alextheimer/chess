#ifndef GAME_MOVE_H_
#define GAME_MOVE_H_

#include "board/board.h"

#include <optional>

namespace game {

struct Move {
    board::Square from;
    board::Square to;
    std::string toString() const;
};

bool operator==(const Move& lhs, const Move& rhs);

std::ostream& operator<<(std::ostream& out, const Move& move);
std::size_t getPieceMoves(board::Board& board, board::PieceColor color, board::Square square, Move * buffer);
std::size_t getAllMoves(board::Board& board, board::PieceColor color, Move * buffer);
void makeMove(board::Board& board, Move& move);
void unmakeMove(board::Board& board, Move& move, board::Piece replacement);

}

#endif  // GAME_MOVE_H_
