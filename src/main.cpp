// Copyright 2021 Alex Theimer

#include <unordered_map>
#include <iostream>
#include <ctime>
#include <cstdlib>

#include "board/board.h"
#include "game/game.h"
#include "player/human.h"
#include "player/computer/computer.h"
#include "util/assert.h"

int main(int argc, char *argv[]) {
    board::Board board(game::INIT_PIECE_MAP);
    player::Computer player1("RoboJim9000"), player2("RoboTim9000");
    game::Game game(&board, &player1, &player2);

    std::srand(std::time(NULL));

    while (!game.isEnded()) {
        game.renderBoard(std::cout);
        game.runPly();
    }

    game::Player& winner = game.getWinner();
    std::cout << winner.getName() << " wins!";
}
