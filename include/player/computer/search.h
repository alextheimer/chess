// Copyright 2021 Alex Theimer

#ifndef PLAYER_COMPUTER_SEARCH_H_
#define PLAYER_COMPUTER_SEARCH_H_

#include <unordered_map>

#include "game/game.h"

namespace player {
namespace computer {

// TODO(theimer): directly associate players with colors?
// TODO(theimer): typedef scores; remove Zob dependency-- just use size_t

/*
Searches the tree of possible Boards to a certain depth, and returns
the best-possible Move that an opponent might allow.

Note that there might be multiple "best-possible" Moves. When this
happens, one of the "best-possible" Moves is ***RANDOMLY*** returned.

@param color: color of the player to plan the move
@param depth: must be >= 1
@param board_heuristic: accepts a Board and color, and returns a
    heuristic value of the Board from the "color" player's perspective
@param score_cache: Can contain existing pairs.
                    Will be updated with additional scores.
*/
game::Move alphaBetaSearch(
        const board::Board& board, board::PieceColor color,
        std::size_t depth,
        int64_t (*board_heuristic)(const board::Board&,
                                   board::PieceColor color),
        std::unordered_map<board::ZobHash, int64_t>* score_cache);

/*
Returns the negative of the count of oppositely-colored pieces.
i.e. "More enemies = worse."

@param color: the color of the "perspective" from which the Board is
              evaluated. Example: if color == BLACK, then a Board is
              evaluated worse for every additional WHITE piece.
*/
int64_t basicBoardHeuristic(const board::Board& board, board::PieceColor color);

}  // namespace computer
}  // namespace player

#endif  // PLAYER_COMPUTER_SEARCH_H_
