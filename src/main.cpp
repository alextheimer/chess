// Copyright 2021 Alex Theimer

#include <unordered_map>
#include <iostream>

#include "board/board.h"
#include "game/game.h"
#include "player/human.h"
#include "util/assert.h"

int main(int argc, char *argv[]) {
    board::Board board(game::INIT_PIECE_MAP);
    player::Human player1("Jimbo Slice"), player2("Jenny Craig");
    game::Game game(&board, &player1, &player2);

    while (!game.isEnded()) {
        game.renderBoard(std::cout);
        game.runPly();
    }

    player::Human& winner = dynamic_cast<player::Human&>(game.getWinner());
    std::cout << winner.getName() << " wins!";
}
