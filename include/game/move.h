#ifndef GAME_MOVE_H_
#define GAME_MOVE_H_

#include "board/board.h"

#include <optional>

using namespace board;

namespace game {

struct Move {
    Square from;
    Square to;
    std::string toString() const;
};

bool operator==(const Move& lhs, const Move& rhs);

std::ostream& operator<<(std::ostream& out, const Move& move);
std::size_t getPieceMoves(Board& board, PieceColor color, Square square, Move * buffer);
std::size_t getAllMoves(Board& board, PieceColor color, Move * buffer);
void makeMove(Board& board, Move& move);
void unmakeMove(Board& board, Move& move, Piece replacement);

}

#endif  // GAME_MOVE_H_
