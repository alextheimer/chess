#include <unordered_map>
#include <iostream>

#include "board/board.h"
#include "game/game.h"
#include "player/human.h"
#include "util/assert.h"

int main(int argc, char *argv[]) {
    board::Board board(game::INIT_PIECE_MAP);

    player::Human player1, player2;

    game::Game game(board, player1, player2);

    game.renderBoard(std::cout);

    game.runPly();

    game.renderBoard(std::cout);
}
