// Copyright 2021 Alex Theimer

#ifndef PLAYER_COMPUTER_SEARCH_H_
#define PLAYER_COMPUTER_SEARCH_H_

#include "game/game.h"

namespace player {
namespace computer {

// TODO(theimer): document
game::Move alphaBetaSearch(const board::Board& board,
                           board::PieceColor color,
                           std::size_t depth,
                           int64_t (*board_heuristic)(const board::Board&,
                                                      board::PieceColor color));

// TODO(theimer): document
int64_t basicBoardHeuristic(const board::Board& board, board::PieceColor color);

}  // namespace computer
}  // namespace player

#endif  // PLAYER_COMPUTER_SEARCH_H_
