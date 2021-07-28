// Copyright 2021 Alex Theimer

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unordered_map>

#include "board/board.h"
#include "game/game.h"
#include "player/human.h"
#include "player/computer/computer.h"
#include "util/assert.h"
#include "util/macro.h"

int main(int argc, char *argv[]) {
    UNUSED(argc);
    UNUSED(argv);

    board::Board board(game::INIT_PIECE_MAP);
    player::Computer player1("RoboJim9000"), player2("RoboTim9000");
    game::Game game(&board, &player1, &player2);

    std::srand(std::time(NULL));

    while (!game.isEnded()) {
        game.renderBoard(std::cout);
        game.runPly();
    }

    game::Player& winner = game.getWinner();
    std::cout << winner.getName() << " wins!" << std::endl;
}
