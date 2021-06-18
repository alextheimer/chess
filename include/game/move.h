#include "board/board.h"

#include <optional>

using namespace board;

namespace game {

std::size_t getPieceMoves(Board& board, PieceColor color, Square square, Move * buffer);
std::size_t getAllMoves(Board& board, PieceColor color, Move * buffer);
void makeMove(Board& board, Move& move);
void unmakeMove(Board& board, Move& move, Piece replacement);

}
