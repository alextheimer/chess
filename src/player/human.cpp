// Copyright 2021 Alex Theimer

#include "player/human.h"

#include <iostream>
#include <limits>

using board::Board;
using board::Square;

using game::Move;

/*
Prompt the player for a Move.

Delivers additional error prompts if the Human Player gives incorrect input.
*/
Move promptPlayerForMove() {
    std::size_t row_from, col_from, row_to, col_to;
    while (true) {
        std::cout << "Enter row_from col_from row_to col_to: ";
        if (!(std::cin >> row_from >> col_from >> row_to >> col_to)) {
            std::cout << "Could not parse input!" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (!Square::isValidDims(row_from, col_from)) {
           std::cout << "Invalid 'from' Square dimensions: ("
                     << std::to_string(row_from)
                     << ", " << std::to_string(row_from) << ")" << std::endl;
        } else if (!Square::isValidDims(row_to, col_to)) {
           std::cout << "Invalid 'to' Square dimensions: ("
                     << std::to_string(row_to)
                     << ", " << std::to_string(row_to) << ")" << std::endl;
        } else {
            // dims are good!
            return (Move){ Square(row_from, col_from), Square(row_to, col_to) };
        }
    }
}

player::Human::Human(std::string name) : name_(name) {
    // intentionally blank
}

std::string player::Human::getName() const {
    return name_;
}

Move player::Human::getMove(const Board& board, board::PieceColor color) {
    Move move = promptPlayerForMove();
    while (!game::isValidMove(board, color, move)) {
        std::cout << "Invalid move!" << std::endl;
        move = promptPlayerForMove();
    }
    return move;
}
